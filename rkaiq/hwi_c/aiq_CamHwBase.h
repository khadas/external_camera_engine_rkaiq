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
#ifndef _AIQ_CAMHW_BASE_H_
#define _AIQ_CAMHW_BASE_H_

#include "c_base/aiq_cond.h"
#include "hwi_c/aiq_aiIspLoader.h"
#include "hwi_c/aiq_ispParamsSplitter.h"
#include "hwi_c/aiq_sensorHw.h"
#include "hwi_c/aiq_spStreamProcUnit.h"
#include "hwi_c/aiq_stream.h"
#include "algos/aiisp/rk_aiisp.h"
#include "include/iq_parser_v2/aec_head.h"
#include "include/iq_parser_v2/af_head.h"
#include "include/iq_parser_v2/sensorinfo_head.h"
#include "xcore_c/aiq_v4l2_buffer.h"
#include "xcore_c/aiq_v4l2_device.h"

XCAM_BEGIN_DECLARE

extern bool g_mIsMultiIspMode;

#define ISP20HW_SUBM (0x1)

#define MAX_PARAMS_QUEUE_SIZE       5
#define ISP_PARAMS_EFFECT_DELAY_CNT 2
#define CAM_INDEX_FOR_1608          8

typedef struct AiqCifSclStream_s AiqCifSclStream_t;
typedef struct AiqIspParamsCvt_s AiqIspParamsCvt_t;
typedef struct AiqMap_s AiqMap_t;
typedef struct AiqList_s AiqList_t;
typedef struct AiqPool_s AiqPool_t;
typedef struct AiqPdafStreamProcUnit_s AiqPdafStreamProcUnit_t;
typedef struct AiqRawStreamCapUnit_s AiqRawStreamCapUnit_t;
typedef struct AiqRawStreamProcUnit_s AiqRawStreamProcUnit_t;
typedef struct AiqManager_s AiqManager_t;

enum cam_hw_state_e {
    CAM_HW_STATE_INVALID,
    CAM_HW_STATE_INITED,
    CAM_HW_STATE_PREPARED,
    CAM_HW_STATE_STARTED,
    CAM_HW_STATE_PAUSED,
    CAM_HW_STATE_STOPPED,
};
enum ircut_state_e {
    IRCUT_STATE_CLOSED, /* close ir-cut,meaning that infrared ray can be received */
    IRCUT_STATE_CLOSING,
    IRCUT_STATE_OPENING,
    IRCUT_STATE_OPENED, /* open ir-cut,meaning that only visible light can be received */
};

typedef struct calib_infos_s {
    struct {
        bool enable;
        bool motion_detect_en;
    } mfnr;

    struct {
        CalibDb_IrisTypeV2_t IrisType;
    } aec;

    struct {
        CalibDbV2_Af_VcmCfg_t vcmcfg;
        CalibDbV2_Af_LdgParam_t ldg_param;
        CalibDbV2_Af_HighLightParam_t highlight;
    } af;

    struct {
        CalibDb_DcgSetV2_t CISDcgSet;
        CalibDb_ExpUpdate_CombV2_t CISExpUpdate;
    } sensor;
} calib_infos_t;

enum mipi_stream_idx {
    MIPI_STREAM_IDX_0   = 1,
    MIPI_STREAM_IDX_1   = 2,
    MIPI_STREAM_IDX_2   = 4,
    MIPI_STREAM_IDX_ALL = 7,
};

typedef struct drv_share_mem_ctx_s {
    void* ops_ctx;
    void* mem_info;
    rk_aiq_drv_share_mem_type_t type;
} drv_share_mem_ctx_t;

enum ISP_STREAM_STATUS_E {
    ISP_STREAM_STATUS_INVALID,
    ISP_STREAM_STATUS_STREAM_ON,
    ISP_STREAM_STATUS_STREAM_OFF,
};

// FIXME: Move it from another "aiq_core/thumbnails.h"
typedef enum rkaiq_stream_type_e {
    RKISP_STREAM_NONE,
    RKISP20_STREAM_MIPITX_S,
    RKISP20_STREAM_MIPITX_M,
    RKISP20_STREAM_MIPITX_L,
    RKISP20_STREAM_SP_RAW,
    RKISP20_STREAM_SP,
    RKISP20_STREAM_TNR,
    RKISP20_STREAM_NR,
    RKISP20_STREAM_FEC,
    // RKISP21_SP,
    RKISP_STREAM_MAX,
} rkaiq_stream_type_t;

typedef struct AiqDevBufCntsInfo_s {
    char _devName[16];
    int _bufCnts;
} AiqDevBufCntsInfo_t;

// FIXME: share 1608 data ptr(aiq/rawdata)
typedef struct sensor_info_share_s {
    AiqRawStreamProcUnit_t* raw_proc_unit[CAM_INDEX_FOR_1608];  // bind rx by camId
    aiq_autoptr_t* raw_cap_unit;                                // save 1st tx obj addr
    char reference_name[64];                                    // save vicap name(for 1608)
    rk_aiq_cif_info_t* reference_mipi_cif;                      // vicap inf (for 1608)
    // us: union_stream
    int us_open_cnt;     // for hwi open(1608)
    int us_prepare_cnt;  // for rawCap buffer manage(1608).
    int us_stream_cnt;   // mark cnt. on: ++, off: --
    int us_stop_cnt;     // last sensor stop
    // tracking opened sensor num
    int en_sns_num;                    // Record the number of open sensors
    int first_en[CAM_INDEX_FOR_1608];  // set/get fmt for 1608 sensor
} sensor_info_share_t;

typedef struct SnsFullInfoWraps_s {
    char key[32];
    rk_sensor_full_info_t data;
    struct SnsFullInfoWraps_s* next;
} SnsFullInfoWraps_t;

typedef struct AiqCamHwBase_s {
    AiqSensorHw_t* _mSensorDev;
    AiqIspParamsCvt_t* _mIspParamsCvt;
    AiqV4l2SubDevice_t* mIspCoreDev;
    AiqV4l2Device_t* mIspStatsDev;
    AiqV4l2Device_t* mIspParamsDev;
    AiqLensHw_t* mLensDev;
    AiqV4l2SubDevice_t* mIrcutDev;
    AiqV4l2SubDevice_t* _cif_csi2_sd;
    AiqV4l2Device_t* mIspSpDev;
    AiqV4l2SubDevice_t* mVicapItfDev;
    AiqHwResListener_t _hwResListener;
    calib_infos_t _cur_calib_infos;
    int _hdr_mode;
    AiqMutex_t _isp_params_cfg_mutex;
    int _state;
    volatile bool _is_exit;
    bool _linked_to_isp;
    bool _linked_to_1608;
    bool _linked_to_serdes;
    char sns_name[32];
    uint64_t _isp_module_ens;
    bool mNoReadBack;
    rk_aiq_rotation_t _sharp_fbc_rotation;

    rk_aiq_ldch_share_mem_info_t ldch_mem_info_array[2 * ISP2X_MESH_BUF_NUM];
    rk_aiq_cac_share_mem_info_t cac_mem_info_array[2 * ISP3X_MESH_BUF_NUM];
    rk_aiq_dbg_share_mem_info_t dbg_mem_info_array[2 * RKISP_INFO2DDR_BUF_MAX];
    drv_share_mem_ctx_t _ldch_drv_mem_ctx;
    drv_share_mem_ctx_t _cac_drv_mem_ctx;
    drv_share_mem_ctx_t _dbg_drv_mem_ctx;
    isp_drv_share_mem_ops_t _drv_share_mem_ops;
    AiqMutex_t _mem_mutex;
    rk_aiq_rect_t _crop_rect;
    uint32_t _ds_width;
    uint32_t _ds_heigth;
    uint32_t _ds_width_align;
    uint32_t _ds_heigth_align;
    uint32_t _exp_delay;
    rk_aiq_lens_descriptor _lens_des;
    // isp
    AiqStatsStream_t* mIspStatsStream;
    AiqStream_t* mIspParamStream;
    AiqSofEvtStream_t* mIspSofStream;
#if defined(RKAIQ_ENABLE_SPSTREAM)
    AiqSPStreamProcUnit_t* mSpStreamUnit;
#endif
    AiqStreamEventPollThread_t* mIspStremEvtTh;
    AiqRawStreamCapUnit_t* mRawCapUnit;
    AiqRawStreamProcUnit_t* mRawProcUnit;
    AiqCifSclStream_t* mCifScaleStream;
    AiqPdafStreamProcUnit_t* mPdafStreamUnit;
    AiqMap_t* _effecting_ispparam_map;
    AiqPool_t* mEffectIspParamsPool;
    // <char*, int>
    AiqDevBufCntsInfo_t* mDevBufCntMap;
    int mDevBufCntMap_size;
    rk_aiq_rkrawstream_info_t mRawStreamInfo;
    bool use_rkrawstream;
    int mWorkingMode;
    const CamCalibDbV2Context_t* mCalibDbV2;
    bool mKpHwSt;
    int mCamPhyId;
    bool mIsGroupMode;
    bool mIsMain;
    // if phsical link between vicap and isp, only isp3x support now
    bool mVicapIspPhyLinkSupported;
    AiqIspParamsSplitter_t* mParamsSplitter;
    int _isp_stream_status;
    rk_sensor_pdaf_info_t mPdafInfo;
    AiqMutex_t _stop_cond_mutex;
    // TODO: Sync(1608 sensor) sdk hwEvt cb
    AiqCond_t _sync_done_cond;
    uint32_t _curIspParamsSeq;
    bool _not_skip_first;
    uint32_t mAweekId;
    void* _skipped_params;
    void* _first_awb_param;
    void* _first_awb_cfg;
    bool mIsListenStrmEvt;
    rk_aiq_tb_info_t mTbInfo;
    bool mIsOnlineByWorkingMode;
    AiqPollCallback_t mPollCb;
    bool mIsFake;
    AiqManager_t* rkAiqManager;
    XCamReturn (*prepare)(AiqCamHwBase_t* pCamHw, uint32_t width, uint32_t height, int mode,
                          int t_delay, int g_delay);
    // internal override
    void (*updateEffParams)(AiqCamHwBase_t* pCamHw, void* params, void* ori_params);
    bool (*processTb)(AiqCamHwBase_t* pCamHw, void* params);

    // AIISP
    bool use_aiisp;
    AiqAiIspStream_t* mIspAiispStream;
    rk_aiq_aiisp_cfg_t mAiisp_cfg;
    rk_aiisp_param* aiisp_param;
    struct AiispLibrary lib_aiisp_;
    XCamReturn (*setAiispMode)(AiqCamHwBase_t* pCamHw, rk_aiq_aiisp_cfg_t* aiisp_cfg);
    XCamReturn (*read_aiisp_result)(AiqCamHwBase_t* pCamHw);
    XCamReturn (*get_aiisp_bay3dbuf)(AiqCamHwBase_t* pCamHw);
    XCamReturn (*aiisp_processing)(AiqCamHwBase_t* pCamHw, rk_aiq_aiisp_t* aiisp_evt);
} AiqCamHwBase_t;

rk_aiq_static_info_t* AiqCamHw_getStaticCamHwInfo(const char* sns_ent_name, uint16_t index);
rk_aiq_static_info_t* AiqCamHw_getStaticCamHwInfoByPhyId(const char* sns_ent_name, uint16_t);
XCamReturn AiqCamHw_clearStaticCamHwInfo();
XCamReturn AiqCamHw_initCamHwInfos();
XCamReturn AiqCamHw_selectIqFile(const char* sns_ent_name, char* iqfile_name);
const char* AiqCamHw_getBindedSnsEntNmByVd(const char* vd);

void AiqCamHw_setCalib(AiqCamHwBase_t* pCamHw, const CamCalibDbV2Context_t* calibv2);
XCamReturn AiqCamHw_prepare(AiqCamHwBase_t* pCamHw, uint32_t width, uint32_t height, int mode,
                            int t_delay, int g_delay);
XCamReturn AiqCamHw_start(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_stop(AiqCamHwBase_t* pCamHw);
void AiqCamHw_clean(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_pause(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_resume(AiqCamHwBase_t* pCamHw);
void AiqCamHw_keepHwStAtStop(AiqCamHwBase_t* pCamHw, bool ks);
XCamReturn AiqCamHw_swWorkingModeDyn(AiqCamHwBase_t* pCamHw, int mode);
XCamReturn AiqCamHw_setIspStreamMode(AiqCamHwBase_t* pCamHw, rk_isp_stream_mode_t mode);
rk_isp_stream_mode_t AiqCamHw_getIspStreamMode(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_getSensorModeData(AiqCamHwBase_t* pCamHw, const char* sns_ent_name,
                                      rk_aiq_exposure_sensor_descriptor* sns_des);
#if 0
XCamReturn AiqCamHw_applyAnalyzerResult(AiqCamHwBase_t* pCamHw, aiq_params_base_t* pAlgoRes,
                                        bool sync);
#endif
XCamReturn AiqCamHw_applyAnalyzerResultList(AiqCamHwBase_t* pCamHw, AiqList_t* pAlgoResList);
XCamReturn AiqCamHw_setHwResListener(AiqCamHwBase_t* pCamHw, AiqHwResListener_t* resListener);
XCamReturn AiqCamHw_setModuleCtl(AiqCamHwBase_t* pCamHw, rk_aiq_module_id_t mId, bool mod_en);
XCamReturn AiqCamHw_getModuleCtl(AiqCamHwBase_t* pCamHw, rk_aiq_module_id_t mId, bool* mod_en);
XCamReturn AiqCamHw_notify_capture_raw(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_capture_raw_ctl(AiqCamHwBase_t* pCamHw, capture_raw_t type, int count,
                                    const char* capture_dir, char* output_dir);
XCamReturn AiqCamHw_setSensorFlip(AiqCamHwBase_t* pCamHw, bool mirror, bool flip, int skip_frm_cnt);
XCamReturn AiqCamHw_getSensorFlip(AiqCamHwBase_t* pCamHw, bool* mirror, bool* flip);
XCamReturn AiqCamHw_getSensorCrop(AiqCamHwBase_t* pCamHw, rk_aiq_rect_t* rect);
XCamReturn AiqCamHw_setSensorCrop(AiqCamHwBase_t* pCamHw, rk_aiq_rect_t* rect);
XCamReturn AiqCamHw_getZoomPosition(AiqCamHwBase_t* pCamHw, int* position);
XCamReturn AiqCamHw_getLensVcmCfg(AiqCamHwBase_t* pCamHw, rk_aiq_lens_vcmcfg* lens_cfg);
XCamReturn AiqCamHw_setLensVcmCfg(AiqCamHwBase_t* pCamHw, rk_aiq_lens_vcmcfg* lens_cfg);
XCamReturn AiqCamHw_FocusCorrection(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_ZoomCorrection(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_setAngleZ(AiqCamHwBase_t* pCamHw, float angleZ);
XCamReturn AiqCamHw_getFocusPosition(AiqCamHwBase_t* pCamHw, int* position);
void AiqCamHw_getShareMemOps(AiqCamHwBase_t* pCamHw, isp_drv_share_mem_ops_t** mem_ops);
XCamReturn AiqCamHw_getEffectiveIspParams(AiqCamHwBase_t* pCamHw,
                                          aiq_isp_effect_params_t** ispParams, uint32_t frame_id);
uint64_t AiqCamHw_getIspModuleEnState(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_get_sp_resolution(AiqCamHwBase_t* pCamHw, int* width, int* height,
                                      int* aligned_w, int* aligned_h);
void AiqCamHw_setCamPhyId(AiqCamHwBase_t* pCamHw, int phyId);
int AiqCamHw_getCamPhyId(AiqCamHwBase_t* pCamHw);
#define AiqCamHw_setGroupMode(pCamHw, bGroup, bMain) \
        (pCamHw)->mIsGroupMode = bGroup; (pCamHw)->mIsMain = bMain

#if RKAIQ_HAVE_PDAF
bool AiqCamHw_get_pdaf_support(AiqCamHwBase_t* pCamHw);
PdafSensorType_t AiqCamHw_get_pdaf_type(AiqCamHwBase_t* pCamHw);
#endif
void AiqCamHw_setTbInfo(AiqCamHwBase_t* pCamHw, rk_aiq_tb_info_t* info);

typedef struct AiqDevBufCnt_s {
    char* _dev_name;
    int cnts;
} AiqDevBufCnt_t;

void AiqCamHw_setDevBufCnt(AiqCamHwBase_t* pCamHw, AiqDevBufCnt_t* devBufCntsInfo, int cnt);
XCamReturn AiqCamHw_reset_hardware(AiqCamHwBase_t* pCamHw);

const char* AiqCamHw_rawReproc_preInit(const char* isp_driver, const char* offline_sns_ent_name);
XCamReturn AiqCamHw_rawReproc_genIspParams(AiqCamHwBase_t* pCamHw, uint32_t sequence,
                                           rk_aiq_frame_info_t* offline_finfo, int mode);
XCamReturn AiqCamHw_rawReProc_prepare(AiqCamHwBase_t* pCamHw, uint32_t sequence,
                                      rk_aiq_frame_info_t* offline_finfo);
void AiqCamHw_setRawStreamInfo(AiqCamHwBase_t* pCamHw, rk_aiq_rkrawstream_info_t* info);
XCamReturn AiqCamHw_waitLastSensorDone(AiqCamHwBase_t* pCamHw);
XCamReturn AiqCamHw_setExposureParams(AiqCamHwBase_t* pCamHw, AiqAecExpInfoWrapper_t* expPar);
void AiqCamHw_setMulCamConc(AiqCamHwBase_t* pCamHw, bool cc);
void AiqCamHw_make_ispHwEvt(AiqCamHwBase_t* pCamHw, Aiqisp20Evt_t* pEvt, uint32_t sequence,
                            int type, int64_t timestamp);
XCamReturn AiqCamHw_notify_sof(AiqCamHwBase_t* pCamHw, AiqHwEvt_t* sof_evt);

XCamReturn AiqCamHwBase_init(AiqCamHwBase_t* pCamHw, const char* sns_ent_name);
void AiqCamHwBase_deinit(AiqCamHwBase_t* pCamHw);

AiqSensorExpInfo_t* Aiqisp20Evt_getExpInfoParams(Aiqisp20Evt_t* pEvt, uint32_t frameId);
XCamReturn AiqCamHw_setVicapStreamMode(AiqCamHwBase_t* pCamHw, int mode, bool is_single_mode);
rk_sensor_full_info_t* AiqCamHw_getFullSnsInfo(const char* sensor_name);
aiq_isp_effect_params_t* AiqCamHw_getParamsForEffMap(AiqCamHwBase_t* pCamHw, uint32_t frame_id);

#define CamHW_setManager(pMan, pRkAiqManager) \
        (pMan)->rkAiqManager = pRkAiqManager

XCamReturn AiqCamHw_setUserOtpInfo(AiqCamHwBase_t* pCamHw, rk_aiq_user_otp_info_t* otp_info);
#define AiqCamHw_setListenStrmEvt(pCamHw, isListen) \
        (pCamHw)->mIsListenStrmEvt = isListen

XCAM_END_DECLARE

#endif
