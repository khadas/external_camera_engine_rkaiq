/*
 *  Copyright (c) 2024 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "hwi_c/aiq_stream.h"

#include <fcntl.h>

#include "c_base/aiq_thread.h"
#include "include/common/rkcif-config.h"
#include "include/common/rkisp2-config.h"

static const int g_default_poll_timeout = -1;

XCamReturn RkPollThread_poll_buffer_loop(AiqPollThread_t* pTh);
XCamReturn RkPollThread_start(AiqPollThread_t* pTh);
XCamReturn RkPollThread_stop(AiqPollThread_t* pTh);
static void _destroy_stop_fds(AiqPollThread_t* pTh);
extern void _notify_isp_stream_status(AiqCamHwBase_t* pCamHw, bool on);

static const char* g_poll_type_to_str[ISP_POLL_POST_MAX] = {
    "luma_poll",
    "isp_3a_stats_poll",
    "isp_param_poll",
    "ispp_fec_param_poll",
    "ispp_tnr_param_poll",
    "ispp_nr_param_poll",
    "ispp_tnr_stats_poll",
    "ispp_nr_stats_poll",
    "isp_sof_poll",
    "isp_tx_poll",
    "isp_rx_poll",
    "isp_sp_poll",
    "isp_pdaf_poll",
    "isp_gain",
    "isp_nr_img",
    "ispp_gain_kg",
    "ispp_gain_wr",
    "isp_stream_sync_poll",
    "vicap_stream_on_evt",
    "vicap_reset_evt",
    "vicap_with_rk1608_reset_evt",
    "vicap_scale_poll",
};

static bool _RkPollThread_poll_loop(void* args) {
    AiqPollThread_t* pTh = (AiqPollThread_t*)args;
    XCamReturn ret       = pTh->poll_loop(pTh);
    if (ret == XCAM_RETURN_NO_ERROR || ret == XCAM_RETURN_ERROR_TIMEOUT ||
        ret == XCAM_RETURN_BYPASS)
        return true;
    return false;
}

XCamReturn AiqPollThread_init(AiqPollThread_t* pTh, const char* thName, int type,
                              AiqV4l2Device_t* pDev, AiqStream_t* pStream) {
    pTh->_poll_stop_fd[0] = -1;
    pTh->_poll_stop_fd[1] = -1;
    pTh->_stream          = pStream;
    pTh->_dev             = pDev;
    pTh->_dev_type        = type;
    pTh->poll_loop        = RkPollThread_poll_buffer_loop;
    pTh->start            = RkPollThread_start;
    pTh->stop             = RkPollThread_stop;
    pTh->_thread          = aiqThread_init(thName, _RkPollThread_poll_loop, pTh);
    return XCAM_RETURN_NO_ERROR;
}

void AiqPollThread_deinit(AiqPollThread_t* pTh) {
    if (pTh->stop) pTh->stop(pTh);
    if (pTh->_thread) aiqThread_deinit(pTh->_thread);
}

static void _destroy_stop_fds(AiqPollThread_t* pTh) {
    if (pTh->_poll_stop_fd[1] != -1 || pTh->_poll_stop_fd[0] != -1) {
        close(pTh->_poll_stop_fd[0]);
        close(pTh->_poll_stop_fd[1]);
        pTh->_poll_stop_fd[0] = -1;
        pTh->_poll_stop_fd[1] = -1;
    }
}

XCamReturn _create_stop_fds(AiqPollThread_t* pTh) {
    int status     = 0;
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    _destroy_stop_fds(pTh);

    status = pipe(pTh->_poll_stop_fd);
    if (status < 0) {
        XCAM_LOG_ERROR("Failed to create ispp poll stop pipe: %s", strerror(errno));
        ret = XCAM_RETURN_ERROR_UNKNOWN;
        goto exit_error;
    }

    /**
     * make the reading end of the pipe non blocking.
     * This helps during flush to read any information left there without
     * blocking
     */
    status = fcntl(pTh->_poll_stop_fd[0], F_SETFL, O_NONBLOCK);
    if (status < 0) {
        XCAM_LOG_ERROR("Fail to set event ispp stop pipe flag: %s", strerror(errno));
        ret = XCAM_RETURN_ERROR_UNKNOWN;
        goto exit_error;
    }

    return XCAM_RETURN_NO_ERROR;
exit_error:
    _destroy_stop_fds(pTh);
    return ret;
}

XCamReturn RkPollThread_start(AiqPollThread_t* pTh) {
    if (_create_stop_fds(pTh)) {
        XCAM_LOG_ERROR("create stop fds failed !");
        return XCAM_RETURN_ERROR_UNKNOWN;
    }

    bool ret = aiqThread_start(pTh->_thread);

    return ret ? XCAM_RETURN_NO_ERROR : XCAM_RETURN_ERROR_FAILED;
}

XCamReturn RkPollThread_stop(AiqPollThread_t* pTh) {
    LOGI_CAMHW_SUBM(ISP20HW_SUBM, "RkPollThread %s:%s stop", pTh->_thread->_name, pTh->_dev->_name);
    if (pTh->_poll_stop_fd[1] != -1) {
        char buf          = 0xf;  // random value to write to flush fd.
        unsigned int size = write(pTh->_poll_stop_fd[1], &buf, sizeof(char));
        if (size != sizeof(char)) {
            XCAM_LOG_WARNING("Flush write not completed");
        }
    }

    bool ret = aiqThread_stop(pTh->_thread);
    _destroy_stop_fds(pTh);
    LOGI_CAMHW_SUBM(ISP20HW_SUBM, "stop done");
    return XCAM_RETURN_NO_ERROR;
}

void AiqPollThread_setPollCallback(AiqPollThread_t* pTh, AiqPollCallback_t* callback) {
    pTh->_poll_callback = callback;
}

XCamReturn RkPollThread_poll_buffer_loop(AiqPollThread_t* pTh) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    int poll_ret   = 0;
    int stop_fd = -1;

    stop_fd  = pTh->_poll_stop_fd[0];
    poll_ret = pTh->_dev->poll_event(pTh->_dev, g_default_poll_timeout, stop_fd);

    if (poll_ret == POLL_STOP_RET) {
        LOGD_CAMHW_SUBM(ISP20HW_SUBM, "poll buffer stop success !");
        // stop success, return error to stop the poll thread
        return XCAM_RETURN_ERROR_UNKNOWN;
    }

    if (poll_ret < 0 && (errno == EAGAIN || errno == EINTR)) {
        XCAM_LOG_DEBUG("poll buffer event got interrupt(0x%x), continue\n", poll_ret);
        return XCAM_RETURN_ERROR_TIMEOUT;
    } else if (poll_ret < 0) {
        XCAM_LOG_DEBUG("poll buffer event got error(0x%x) exit\n", poll_ret);
        return XCAM_RETURN_ERROR_UNKNOWN;
    } else if (poll_ret == 0) {
        XCAM_LOG_DEBUG("poll buffer event timeout(0x%x), continue\n", poll_ret);
        return XCAM_RETURN_ERROR_TIMEOUT;
    }

    AiqV4l2Buffer_t* pBuf = pTh->_dev->_dequeue_buffer(pTh->_dev);
    if (!pBuf) {
        XCAM_LOG_WARNING("dequeue buffer failed");
        return XCAM_RETURN_BYPASS;
    }

    LOGD_CAMHW_SUBM(ISP20HW_SUBM, "camId: %d, frameId: %d: dequeue buffer on %s\n", pTh->mCamPhyId,
                    AiqV4l2Buffer_getSequence(pBuf), g_poll_type_to_str[pTh->_dev_type]);

    if (pTh->_dev_type == ISP_POLL_TX || pTh->_dev_type == ISP_POLL_RX ||
        pTh->_dev_type == VICAP_POLL_SCL) {
        if (pTh->_poll_callback) {
            AiqHwEvt_t evt;
            evt.type               = pTh->_dev_type;
            evt.frame_id           = AiqV4l2Buffer_getSequence(pBuf);
            evt.mTimestamp         = AiqV4l2Buffer_getTimestamp(pBuf);
            evt.vb                 = &pBuf->_vb;
            if (pTh->_dev_type)
                evt.vb->_buf_type       = evt.type;
            AiqRawStream_t* stream = (AiqRawStream_t*)pTh->_stream;
            AiqV4l2Buffer_setReserved(pBuf, (uint8_t*)(stream->_reserved));
            AiqV4l2Buffer_ref(pBuf);
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &evt,
                                                   ((AiqRawStream_t*)pTh->_stream)->_dev_index);
            AiqV4l2Buffer_unref(pBuf);
        }
    } else if (pTh->_dev_type == ISP_POLL_PDAF_STATS) {
        if (pTh->_poll_callback) {
            AiqHwPdafEvt_t pdaf_evt;
            pdaf_evt._base.type       = pTh->_dev_type;
            pdaf_evt._base.frame_id   = AiqV4l2Buffer_getSequence(pBuf);
            pdaf_evt._base.mTimestamp = AiqV4l2Buffer_getTimestamp(pBuf);
            pdaf_evt._base.vb         = &pBuf->_vb;
            pdaf_evt._base.vb->_buf_type = pTh->_dev_type;
            AiqV4l2Buffer_ref(pBuf);
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &pdaf_evt._base, -1);
            AiqV4l2Buffer_unref(pBuf);
        }
    } else if (pTh->_dev_type == ISP_POLL_3A_STATS) {
        if (pTh->_poll_callback) {
            AiqHwStatsEvt_t stats_evt;
            stats_evt._base.type       = pTh->_dev_type;
            stats_evt._base.frame_id   = AiqV4l2Buffer_getSequence(pBuf);
            stats_evt._base.mTimestamp = AiqV4l2Buffer_getTimestamp(pBuf);
            stats_evt._base.vb         = &pBuf->_vb;
            stats_evt._base.vb->_buf_type = pTh->_dev_type;
            AiqStatsStream_t* stream   = (AiqStatsStream_t*)pTh->_stream;
            stats_evt.mCamHw           = stream->_rx_handle_dev;
            stats_evt.mFocusLensHw     = stream->_focus_handle_dev;
            stats_evt.mIrishw          = stream->_iris_handle_dev;
            stats_evt.mSensorHw        = stream->_event_handle_dev;
            AiqV4l2Buffer_ref(pBuf);
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &stats_evt._base,
                                                   -1);
            AiqV4l2Buffer_unref(pBuf);
        }
    } else {
        if (pTh->_poll_callback) {
            AiqHwEvt_t evt;
            evt.type       = pTh->_dev_type;
            evt.frame_id   = AiqV4l2Buffer_getSequence(pBuf);
            evt.mTimestamp = AiqV4l2Buffer_getTimestamp(pBuf);
            evt.vb         = &pBuf->_vb;
            evt.vb->_buf_type = evt.type;
            AiqV4l2Buffer_ref(pBuf);
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &evt, -1);
            AiqV4l2Buffer_unref(pBuf);
        }
    }

    return ret;
}

static XCamReturn RkEventPollThread_poll_event_loop(AiqPollThread_t* pTh) {
    XCamReturn ret               = XCAM_RETURN_NO_ERROR;
    int poll_ret                 = 0;
    int stop_fd                  = -1;
    AiqEventPollThread_t* pEvtTh = (AiqEventPollThread_t*)pTh;

    stop_fd  = pTh->_poll_stop_fd[0];
    poll_ret = pTh->_dev->poll_event(pTh->_dev, g_default_poll_timeout, stop_fd);

    if (poll_ret == POLL_STOP_RET) {
        LOGI_CAMHW_SUBM(ISP20HW_SUBM, "%s: poll event stop success !", AiqV4l2Device_getDevName(pTh->_dev));
        // stop success, return error to stop the poll thread
        return XCAM_RETURN_ERROR_UNKNOWN;
    }

    if (poll_ret < 0 && (errno == EAGAIN || errno == EINTR)) {
        XCAM_LOG_DEBUG("poll buffer event got interrupt(0x%x), continue\n", poll_ret);
        return XCAM_RETURN_ERROR_TIMEOUT;
    } else if (poll_ret < 0) {
        XCAM_LOG_DEBUG("poll buffer event got error(0x%x) exit\n", poll_ret);
        return XCAM_RETURN_ERROR_UNKNOWN;
    } else if (poll_ret == 0) {
        XCAM_LOG_WARNING("poll event timeout and continue");
        return XCAM_RETURN_ERROR_TIMEOUT;
    }
    xcam_mem_clear(pEvtTh->_event);

    ret = AiqV4l2Device_dqEvt(pTh->_dev, &pEvtTh->_event);
    if (ret != XCAM_RETURN_NO_ERROR) {
        XCAM_LOG_WARNING("dequeue event failed on dev:%s", AiqV4l2Device_getDevName(pTh->_dev));
        return XCAM_RETURN_ERROR_IOCTL;
    }

    LOGD_CAMHW_SUBM(ISP20HW_SUBM,
                    "camId: %d, frameId: %d: dequeue the type(%d) of event on dev: %s _dev_type: %d",
                    pTh->mCamPhyId, pEvtTh->_event.u.frame_sync.frame_sequence, pEvtTh->_event.type,
                    AiqV4l2Device_getDevName(pTh->_dev), pTh->_dev_type);

    if (pTh->_dev_type == ISP_POLL_SOF) {
        if (pTh->_poll_callback && pTh->_stream) {
            AiqHwEvt_t sof_evt;
            sof_evt.frame_id = pEvtTh->_event.u.frame_sync.frame_sequence;
            if (pEvtTh->_event.type == V4L2_EVENT_RESET_DEV) {
                if (((AiqSofEvtStream_t*)(pTh->_stream))->_linked_to_1608) {
                    sof_evt.type = VICAP_WITH_RK1608_RESET_EVT;
                }
                else
                    sof_evt.type = VICAP_RESET_EVT;
            }
            else {
                sof_evt.type = pTh->_dev_type;
            }
            sof_evt.mTimestamp = XCAM_TIMESPEC_2_NSEC(pEvtTh->_event.timestamp);
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &sof_evt, -1);
        }
    }
    else if (pTh->_dev_type == ISP_POLL_AIISP) {
        if (pTh->_poll_callback && pTh->_stream) {
            AiqHwAiispEvt_t aiisp_evt;
            struct rkisp_aiisp_ev_info* aiisp_ev_info = (struct rkisp_aiisp_ev_info*)pEvtTh->_event.u.data;
            LOGD_CAMHW_SUBM(ISP20HW_SUBM, "aiisp: event.type %d _frameid %d\n", pEvtTh->_event.type, aiisp_ev_info->sequence);
            aiisp_evt._height = aiisp_ev_info->height;
            aiisp_evt._base.frame_id = aiisp_ev_info->sequence;
            aiisp_evt.bay3dbuf = ((AiqAiIspStream_t*)pTh->_stream)->bay3dbuf;
            aiisp_evt.iir_address = ((AiqAiIspStream_t*)pTh->_stream)->iir_address;
            aiisp_evt.gain_address = ((AiqAiIspStream_t*)pTh->_stream)->gain_address;
            aiisp_evt.aiisp_address = ((AiqAiIspStream_t*)pTh->_stream)->aiisp_address;

            if (pEvtTh->_event.type != RKISP_V4L2_EVENT_AIISP_LINECNT) {
                aiisp_evt._base.type = pTh->_dev_type;
                LOGE_CAMHW_SUBM(ISP20HW_SUBM,"The type of event is not RKISP_V4L2_EVENT_AIISP_LINECNT!");
            }
            else {
                aiisp_evt._base.type = pTh->_dev_type;
            }
            pTh->_poll_callback->poll_buffer_ready(pTh->_poll_callback->_pCtx, &aiisp_evt._base, -1);
        }
    }

    return ret;
}

XCamReturn AiqEventPollThread_init(AiqEventPollThread_t* pTh, const char* thName, int type,
                                   AiqV4l2Device_t* pDev, AiqStream_t* pStream) {
    XCamReturn ret        = AiqPollThread_init(&pTh->_base, thName, type, pDev, pStream);
    pTh->_base.poll_loop  = RkEventPollThread_poll_event_loop;

    return ret;
}

void AiqEventPollThread_deinit(AiqEventPollThread_t* pTh) { AiqPollThread_deinit(&pTh->_base); }

XCamReturn RkStreamEventPollThread_start(AiqPollThread_t* pTh) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = pTh->_dev->open(pTh->_dev, false);
    if (ret) {
        return ret;
    }
    AiqV4l2Device_subscribeEvt(pTh->_dev, CIFISP_V4L2_EVENT_STREAM_START);
    AiqV4l2Device_subscribeEvt(pTh->_dev, CIFISP_V4L2_EVENT_STREAM_STOP);
    return RkPollThread_start(pTh);
}

XCamReturn RkStreamEventPollThread_stop(AiqPollThread_t* pTh) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = RkPollThread_stop(pTh);
    if (AiqV4l2Device_isOpened(pTh->_dev)) {
        AiqV4l2Device_unsubscribeEvt(pTh->_dev, CIFISP_V4L2_EVENT_STREAM_START);
        AiqV4l2Device_unsubscribeEvt(pTh->_dev, CIFISP_V4L2_EVENT_STREAM_STOP);
        ret = pTh->_dev->close(pTh->_dev);
    }
    return ret;
}

static XCamReturn RkStreamEventPollThread_poll_event_loop(AiqPollThread_t* pTh) {
    XCamReturn ret                         = RkEventPollThread_poll_event_loop(pTh);
    AiqEventPollThread_t* pEvtTh           = (AiqEventPollThread_t*)pTh;
    AiqStreamEventPollThread_t* pStrmEvtTh = (AiqStreamEventPollThread_t*)pTh;

    if (ret == XCAM_RETURN_NO_ERROR) {
        if (pEvtTh->_event.type == CIFISP_V4L2_EVENT_STREAM_START) {
            LOGI_CAMHW_SUBM(ISP20HW_SUBM, "%s: poll stream on evt success",
                            AiqV4l2Device_getDevName(pTh->_dev));
            _notify_isp_stream_status(pStrmEvtTh->_pIsp, true);
        } else if (pEvtTh->_event.type == CIFISP_V4L2_EVENT_STREAM_STOP) {
            LOGI_CAMHW_SUBM(ISP20HW_SUBM, "%s: poll stream off evt success",
                            AiqV4l2Device_getDevName(pTh->_dev));
            _notify_isp_stream_status(pStrmEvtTh->_pIsp, false);
            return XCAM_RETURN_ERROR_UNKNOWN;
        }
    }

    return ret;
}

XCamReturn AiqStreamEventPollThread_init(AiqStreamEventPollThread_t* pTh, char* thName,
                                         const char* pDev, AiqCamHwBase_t* isp) {
    pTh->_subdev = (AiqV4l2SubDevice_t*)aiq_mallocz(sizeof(AiqV4l2SubDevice_t));
    if (!pTh->_subdev) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "%d: alloc fail !", __LINE__);
		return XCAM_RETURN_NO_ERROR;
    }
    AiqV4l2SubDevice_init(pTh->_subdev, pDev);
    XCamReturn ret =
        AiqEventPollThread_init(&pTh->_base, thName, ISP_POLL_ISPSTREAMSYNC, (AiqV4l2Device_t*)pTh->_subdev, NULL);
    pTh->_pIsp                  = isp;
    pTh->_base._base.poll_loop  = RkStreamEventPollThread_poll_event_loop;
    pTh->_base._base.start      = RkStreamEventPollThread_start;
    pTh->_base._base.stop       = RkStreamEventPollThread_stop;
    return ret;
}

void AiqStreamEventPollThread_deinit(AiqStreamEventPollThread_t* pTh) {
    AiqEventPollThread_deinit(&pTh->_base);
	if (pTh->_subdev) {
		AiqV4l2SubDevice_deinit(pTh->_subdev);
		aiq_free(pTh->_subdev);
		pTh->_subdev = NULL;
	}
}

void RKStream_startThreadOnly(AiqStream_t* pStream) {
    AiqPollThread_setCamPhyId(pStream->_poll_thread, pStream->mCamPhyId);
    if (pStream->_dev_type == ISP_POLL_3A_STATS || pStream->_dev_type == ISP_POLL_TX ||
        pStream->_dev_type == ISP_POLL_RX) {
        aiqThread_setPolicy(pStream->_poll_thread->_thread, SCHED_RR);
        aiqThread_setPriority(pStream->_poll_thread->_thread, 20);
    }
    pStream->_poll_thread->start(pStream->_poll_thread);
}

void RKStream_startDeviceOnly(AiqStream_t* pStream) {
    if (!AiqV4l2Device_isActivated(pStream->_dev))
        pStream->_dev->start(pStream->_dev, pStream->_dev_prepared);
}

void RKStream_start(AiqStream_t* pStream) {
    RKStream_startDeviceOnly(pStream);
    RKStream_startThreadOnly(pStream);
}

void RKStream_stopThreadOnly(AiqStream_t* pStream) {
    pStream->_poll_thread->stop(pStream->_poll_thread);
}

void RKStream_stopDeviceOnly(AiqStream_t* pStream) {
    pStream->_dev->stop(pStream->_dev);
    pStream->_dev_prepared = false;
}

void RKStream_stop(AiqStream_t* pStream) {
    RKStream_stopThreadOnly(pStream);
    RKStream_stopDeviceOnly(pStream);
}

void RKStream_pause(AiqStream_t* pStream) {}

void RKStream_resume(AiqStream_t* pStream) {}

void RKStream_set_device_prepared(AiqStream_t* pStream, bool prepare) {
    pStream->_dev_prepared = prepare;
}

bool RKStream_setPollCallback(AiqStream_t* pStream, AiqPollCallback_t* callback) {
	AiqPollThread_setPollCallback(pStream->_poll_thread, callback);
    return true;
}

XCamReturn RKStream_getFormat(AiqStream_t* pStream, struct v4l2_format* format) {
    return pStream->_dev->get_format(pStream->_dev, format);
}

XCamReturn RKStream_getSubdevFormat(AiqStream_t* pStream, struct v4l2_subdev_format* format) {
    return ((AiqV4l2SubDevice_t*)pStream->_dev)
        ->getFormat((AiqV4l2SubDevice_t*)(pStream->_dev), format);
}

static void RKStream_setCamPhyId(AiqStream_t* pStream, int phyId)
{
	pStream->mCamPhyId = phyId;
	AiqPollThread_setCamPhyId(pStream->_poll_thread, phyId);
}

XCamReturn AiqStream_init(AiqStream_t* pStream, AiqV4l2Device_t* pDev, int type) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    pStream->_dev      = pDev;
    pStream->_dev_type = type;

    if (type == ISP_POLL_SOF || type == ISP_POLL_AIISP) {
        pStream->_poll_thread = (AiqPollThread_t*)aiq_mallocz(sizeof(AiqEventPollThread_t));
        ret = AiqEventPollThread_init((AiqEventPollThread_t*)(pStream->_poll_thread),
                                      g_poll_type_to_str[type], type, pDev, pStream);
    } else {
        pStream->_poll_thread = (AiqPollThread_t*)aiq_mallocz(sizeof(AiqPollThread_t));
        ret = AiqPollThread_init(pStream->_poll_thread, g_poll_type_to_str[type], type, pDev,
                                 pStream);
    }
    pStream->start               = RKStream_start;
    pStream->startThreadOnly     = RKStream_startThreadOnly;
    pStream->startDeviceOnly     = RKStream_startDeviceOnly;
    pStream->stop                = RKStream_stop;
    pStream->stopThreadOnly      = RKStream_stopThreadOnly;
    pStream->stopDeviceOnly      = RKStream_stopDeviceOnly;
    pStream->pause               = RKStream_pause;
    pStream->resume              = RKStream_resume;
    pStream->set_device_prepared = RKStream_set_device_prepared;
    pStream->setPollCallback     = RKStream_setPollCallback;
    pStream->getFormat           = RKStream_getFormat;
    pStream->getSubDevFormat     = RKStream_getSubdevFormat;
    pStream->setCamPhyId		 = RKStream_setCamPhyId;

    return ret;
}

void AiqStream_deinit(AiqStream_t* pStream) {
    if (pStream->_poll_thread) {
        if (pStream->_dev_type == ISP_POLL_SOF)
            AiqEventPollThread_deinit((AiqEventPollThread_t*)(pStream->_poll_thread));
        else
            AiqPollThread_deinit(pStream->_poll_thread);
        aiq_free(pStream->_poll_thread);
        pStream->_poll_thread = NULL;
    }
}

bool RKStatsStream_set_event_handle_dev(AiqStatsStream_t* pStream, AiqSensorHw_t* dev) {
    pStream->_event_handle_dev = dev;
    return true;
}

typedef struct AiqLensHw_s AiqLensHw_t;

bool RKStatsStream_set_iris_handle_dev(AiqStatsStream_t* pStream, AiqLensHw_t* dev) {
    pStream->_iris_handle_dev = dev;
    return true;
}

bool RKStatsStream_set_focus_handle_dev(AiqStatsStream_t* pStream, AiqLensHw_t* dev) {
    pStream->_focus_handle_dev = dev;
    return true;
}

bool RKStatsStream_set_rx_handle_dev(AiqStatsStream_t* pStream, AiqCamHwBase_t* dev) {
    pStream->_rx_handle_dev = dev;
    return true;
}

XCamReturn AiqStatsStream_init(AiqStatsStream_t* pStream, AiqV4l2Device_t* pDev, int type) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = AiqStream_init(&pStream->_base, pDev, type);

    pStream->set_event_handle_dev = RKStatsStream_set_event_handle_dev;
    pStream->set_iris_handle_dev  = RKStatsStream_set_iris_handle_dev;
    pStream->set_focus_handle_dev = RKStatsStream_set_focus_handle_dev;
    pStream->set_rx_handle_dev    = RKStatsStream_set_rx_handle_dev;

    return ret;
}

void AiqStatsStream_deinit(AiqStatsStream_t* pStream) { AiqStream_deinit(&pStream->_base); }

// AIISP
static XCamReturn set_aiisp_linecnt(AiqAiIspStream_t* pStream, rk_aiq_aiisp_cfg_t aiisp_cfg) {
    int res = -1;
    struct rkisp_aiisp_cfg aiisp_cfg_io;
    aiisp_cfg_io.wr_mode    = aiisp_cfg.wr_mode;
    aiisp_cfg_io.rd_mode    = aiisp_cfg.rd_mode;
    aiisp_cfg_io.wr_linecnt = aiisp_cfg.wr_linecnt;
    aiisp_cfg_io.rd_linecnt = aiisp_cfg.rd_linecnt;
    res = AiqV4l2SubDevice_ioctl(pStream->_base._dev, RKISP_CMD_SET_AIISP_LINECNT, &aiisp_cfg_io);
    if (res) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "set aiisp linecnt failed! %d", res);
        return XCAM_RETURN_ERROR_IOCTL;
    }
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "aiisp wr_linecnt is %d rd_linecnt is %d", aiisp_cfg.wr_linecnt,
                    aiisp_cfg.rd_linecnt);
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn close_aiisp(AiqAiIspStream_t* pStream) {
    int res = -1;
    rk_aiq_aiisp_cfg_t aiisp_cfg;
    aiisp_cfg.wr_linecnt = 0;
    res = AiqV4l2SubDevice_ioctl(pStream->_base._dev, RKISP_CMD_SET_AIISP_LINECNT, &aiisp_cfg);
    if (res) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "close aiisp failed! %d", res);
        return XCAM_RETURN_ERROR_IOCTL;
    }
    int mun_ret = munmap(pStream->iir_address, pStream->bay3dbuf.iir_size);
    mun_ret     = munmap(pStream->gain_address, pStream->bay3dbuf.u.v39.gain_size);
    mun_ret     = munmap(pStream->aiisp_address, pStream->bay3dbuf.u.v39.aiisp_size);
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "close aiisp success");
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn get_aiisp_bay3dbuf(AiqAiIspStream_t* pStream) {
    int res = -1;
    memset(&pStream->bay3dbuf, 0, sizeof(pStream->bay3dbuf));
    res =
        AiqV4l2SubDevice_ioctl(pStream->_base._dev, RKISP_CMD_GET_BAY3D_BUFFD, &pStream->bay3dbuf);
    if (res) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "get aiisp bay3dbuf failed! %d", res);
        return XCAM_RETURN_ERROR_IOCTL;
    }
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "get aiisp bay3dbuf: iir_fd is %d iir_size is %d",
                    pStream->bay3dbuf.iir_fd, pStream->bay3dbuf.iir_size);
    int iir_fd   = pStream->bay3dbuf.iir_fd;
    int iir_size = pStream->bay3dbuf.iir_size;
    pStream->iir_address =
        (char*)mmap(NULL, iir_size, PROT_READ | PROT_WRITE, MAP_SHARED, iir_fd, 0);
    if (MAP_FAILED == pStream->iir_address) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "iir_fd mmap failed");
        return XCAM_RETURN_ERROR_FAILED;
    }
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "iir_fd %d, iir_size %d", iir_fd, iir_size);
    int gain_fd   = pStream->bay3dbuf.u.v39.gain_fd;
    int gain_size = pStream->bay3dbuf.u.v39.gain_size;
    pStream->gain_address =
        (char*)mmap(NULL, gain_size, PROT_READ | PROT_WRITE, MAP_SHARED, gain_fd, 0);
    if (MAP_FAILED == pStream->gain_address) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "gain_fd mmap failed");
        return XCAM_RETURN_ERROR_FAILED;
    }
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "gain_fd %d, gain_size %d", gain_fd, gain_size);
    int aiisp_fd   = pStream->bay3dbuf.u.v39.aiisp_fd;
    int aiisp_size = pStream->bay3dbuf.u.v39.aiisp_size;
    pStream->aiisp_address =
        (char*)mmap(NULL, aiisp_size, PROT_READ | PROT_WRITE, MAP_SHARED, aiisp_fd, 0);
    if (MAP_FAILED == pStream->aiisp_address) {
        LOGE_CAMHW_SUBM(ISP20HW_SUBM, "aiisp_fd mmap failed");
        return XCAM_RETURN_ERROR_FAILED;
    }
    LOGK_CAMHW_SUBM(ISP20HW_SUBM, "aiisp_fd %d, aiisp_size %d", aiisp_fd, aiisp_size);
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn call_aiisp_rd_start(AiqAiIspStream_t* pStream) {
    int res = -1;
    res     = AiqV4l2SubDevice_ioctl(pStream->_base._dev, RKISP_CMD_AIISP_RD_START, NULL);
    if (res) return XCAM_RETURN_ERROR_IOCTL;
    LOGD_CAMHW_SUBM(ISP20HW_SUBM, "call aiisp rd start success");
    return XCAM_RETURN_NO_ERROR;
}

static void AiqAiIspStream_start(AiqStream_t* pStream) {
    AiqAiIspStream_t* pAiispStrm = (AiqAiIspStream_t*)pStream;
    RKStream_start(&pAiispStrm->_base);
    AiqV4l2Device_subscribeEvt(pAiispStrm->_base._dev, RKISP_V4L2_EVENT_AIISP_LINECNT);
}

static void AiqAiIspStream_stop(AiqStream_t* pStream) {
    AiqAiIspStream_t* pAiispStrm = (AiqAiIspStream_t*)pStream;
    RKStream_stopThreadOnly(&pAiispStrm->_base);
    close_aiisp(pAiispStrm);
    AiqV4l2Device_unsubscribeEvt(pAiispStrm->_base._dev, RKISP_V4L2_EVENT_AIISP_LINECNT);
    RKStream_stopDeviceOnly(&pAiispStrm->_base);
}

XCamReturn AiqAiIspStream_init(AiqAiIspStream_t* pStream, AiqV4l2Device_t* pDev, int type) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = AiqStream_init(&pStream->_base, pDev, type);

    pStream->set_aiisp_linecnt   = set_aiisp_linecnt;
    pStream->get_aiisp_bay3dbuf  = get_aiisp_bay3dbuf;
    pStream->call_aiisp_rd_start = call_aiisp_rd_start;
    pStream->close_aiisp         = close_aiisp;
    pStream->_base.start               = AiqAiIspStream_start;
    pStream->_base.stop                = AiqAiIspStream_stop;

    return ret;
}

void AiqAiIspStream_deinit(AiqAiIspStream_t* pStream) { AiqStream_deinit(&pStream->_base); }

void RKSofEventStream_start(AiqStream_t* pStream) {
    AiqSofEvtStream_t* pSofStrm = (AiqSofEvtStream_t*)pStream;

    pStream->_dev->start(pStream->_dev, pStream->_dev_prepared);
    pStream->setCamPhyId(pStream, pStream->mCamPhyId);
    pStream->_poll_thread->start(pStream->_poll_thread);
    AiqV4l2Device_subscribeEvt(pStream->_dev, V4L2_EVENT_FRAME_SYNC);
    if (pSofStrm->_linked_to_1608) {
        if (!pSofStrm->_is_subscribed) {
            AiqV4l2Device_subscribeEvt(pStream->_dev, V4L2_EVENT_RESET_DEV);
            pSofStrm->_is_subscribed = true;
        }
    } else {
        AiqV4l2Device_subscribeEvt(pStream->_dev, V4L2_EVENT_RESET_DEV);
    }
}

void RKSofEventStream_stop(AiqStream_t* pStream) {
    AiqSofEvtStream_t* pSofStrm = (AiqSofEvtStream_t*)pStream;

    pStream->_poll_thread->stop(pStream->_poll_thread);
    AiqV4l2Device_unsubscribeEvt(pStream->_dev, V4L2_EVENT_FRAME_SYNC);
    if (pSofStrm->_linked_to_1608) {
        if (pSofStrm->_is_subscribed) {
            AiqV4l2Device_unsubscribeEvt(pStream->_dev, V4L2_EVENT_RESET_DEV);
            pSofStrm->_is_subscribed = false;
        }
    } else {
        AiqV4l2Device_unsubscribeEvt(pStream->_dev, V4L2_EVENT_RESET_DEV);
    }
    pStream->_dev->stop(pStream->_dev);
}

XCamReturn AiqSofEvtStream_init(AiqSofEvtStream_t* pStream, AiqV4l2Device_t* pDev, int type,
                                bool linkedToRk1608) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret                      = AiqStream_init(&pStream->_base, pDev, type);
    pStream->_linked_to_1608 = linkedToRk1608;
    pStream->_base.start     = RKSofEventStream_start;
    pStream->_base.stop      = RKSofEventStream_stop;

    return ret;
}

void AiqSofEvtStream_deinit(AiqSofEvtStream_t* pStream) { AiqStream_deinit(&pStream->_base); }

void RKRawStream_set_reserved_data(AiqRawStream_t* pStream, int bpp) {
    pStream->_bpp         = bpp;
    pStream->_reserved[0] = pStream->_dev_index;
    pStream->_reserved[1] = bpp;
}

XCamReturn AiqRawStream_init(AiqRawStream_t* pStream, AiqV4l2Device_t* pDev, int type, int index) {
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = AiqStream_init(&pStream->_base, pDev, type);

    pStream->_dev_index        = index;
    pStream->set_reserved_data = RKRawStream_set_reserved_data;

    return ret;
}

void AiqRawStream_deinit(AiqRawStream_t* pStream) { AiqStream_deinit(&pStream->_base); }

XCamReturn AiqPdafStream_init(AiqStream_t* pStream, AiqV4l2Device_t* pDev, int type) {
    return AiqStream_init(pStream, pDev, type);
}

void AiqPdafStream_deinit(AiqStream_t* pStream) { AiqStream_deinit(pStream); }

bool AiqPollThread_setCamPhyId(AiqPollThread_t* pTh, int phyId) {
    pTh->mCamPhyId = phyId;
    return true;
}
