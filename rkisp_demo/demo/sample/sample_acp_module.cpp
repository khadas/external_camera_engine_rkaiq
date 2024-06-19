/*
 *  Copyright (c) 2019 Rockchip Corporation
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

#include "sample_comm.h"

#include "uAPI2/rk_aiq_user_api2_helper.h"
#include <string>

static void sample_acp_usage()
{
    printf("Usage : \n");
    printf("  Module API: \n");
    printf("\t 1) ACP:         Set brightness   & Async .\n");
    printf("\t 2) ACP:         Set brightness   & Sync .\n");
    printf("\t 3) ACP:         Set contrast     & Async .\n");
    printf("\t 4) ACP:         Set contrast     & Sync .\n");
    printf("\t 5) ACP:         Set saturation   & Async.\n");
    printf("\t 6) ACP:         Set saturation   & Sync.\n");
    printf("\t 7) ACP:         Set hue          & Async.\n");
    printf("\t 8) ACP:         Set hue          & Sync.\n");
    printf("\t 9) ACP:         Get acp attrib   & Async.\n");
    printf("\t a) ACP:         Get acp attrib   & Sync.\n");
    printf("  ImgProc API: \n");
    printf("\t b) ACP:         Set Contrast.\n");
    printf("\t c) ACP:         Set Brightness.\n");
    printf("\t d) ACP:         Set Saturation.\n");
    printf("\t e) ACP:         Set Hue.\n");
    printf("\t f) ACP:         newstruct test.\n");
    printf("\n");
    printf("\t h) ACP:         help.\n");
    printf("\t q) ACP:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_acp_info(const void *arg)
{
    printf ("enter ACP module test!\n");
}

/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/

static int sample_set_acp_contrast(const rk_aiq_sys_ctx_t* ctx, uint8_t level)
{
    rk_aiq_uapi2_setContrast(ctx, level);
    return 0;
}

static int sample_get_acp_contrast(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int level;
    rk_aiq_uapi2_getContrast(ctx, &level);
    printf("Get ACP Contrast : %u \n", level);
    return 0;
}

static int sample_set_acp_brightness(const rk_aiq_sys_ctx_t* ctx, uint8_t level)
{
    rk_aiq_uapi2_setBrightness(ctx, level);
    return 0;
}

static int sample_get_acp_brightness(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int level;
    rk_aiq_uapi2_getBrightness(ctx, &level);
    printf("Get ACP Brightness : %u \n", level);
    return 0;
}

static int sample_set_acp_saturation(const rk_aiq_sys_ctx_t* ctx, uint8_t level)
{
    rk_aiq_uapi2_setSaturation(ctx, level);
    return 0;
}

static int sample_get_acp_saturation(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int level;
    rk_aiq_uapi2_getSaturation(ctx, &level);
    printf("Get ACP Saturation : %u \n", level);
    return 0;
}

static int sample_set_acp_hue(const rk_aiq_sys_ctx_t* ctx, uint8_t level)
{
    rk_aiq_uapi2_setHue(ctx, level);
    return 0;
}

static int sample_get_acp_hue(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int level;
    rk_aiq_uapi2_getHue(ctx, &level);
    printf("Get ACP Hue : %u \n", level);
    return 0;
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/

static int sample_acp_set_brightness(const rk_aiq_sys_ctx_t* ctx, uint8_t brightness,
                                     rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  acp_attrib_t attr;
  memset(&attr, 0, sizeof(acp_attrib_t));

  ret = rk_aiq_user_api2_acp_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAcpAttr failed in getting acp attrib!");

  attr.sync.sync_mode   = sync;
  attr.brightness       = brightness;

  //set
  ret = rk_aiq_user_api2_acp_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set acp Attr failed!");
  printf("set acp brightness: %d, done: %d\n\n", brightness, attr.sync.done);

  return 0;
}

static int sample_acp_set_contrast(const rk_aiq_sys_ctx_t* ctx, uint8_t contrast,
                                   rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  acp_attrib_t attr;
  memset(&attr, 0, sizeof(acp_attrib_t));

  ret = rk_aiq_user_api2_acp_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAcpAttr failed in getting acp attrib!");

  attr.sync.sync_mode   = sync;
  attr.contrast         = contrast;

  //set
  ret = rk_aiq_user_api2_acp_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set acp Attr failed!");
  printf("set acp contrast: %d, done: %d\n\n", contrast, attr.sync.done);

  return 0;
}

static int sample_acp_set_saturation(const rk_aiq_sys_ctx_t* ctx, uint8_t saturation,
                                   rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  acp_attrib_t attr;
  memset(&attr, 0, sizeof(acp_attrib_t));

  ret = rk_aiq_user_api2_acp_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAcpAttr failed in getting acp attrib!");

  attr.sync.sync_mode   = sync;
  attr.saturation       = saturation;

  //set
  ret = rk_aiq_user_api2_acp_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set acp Attr failed!");
  printf("set acp saturation: %d, done: %d\n\n", saturation, attr.sync.done);

  return 0;
}

static int sample_acp_set_hue(const rk_aiq_sys_ctx_t* ctx, uint8_t hue,
                              rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  acp_attrib_t attr;
  memset(&attr, 0, sizeof(acp_attrib_t));

  ret = rk_aiq_user_api2_acp_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAcpAttr failed in getting acp attrib!");

  attr.sync.sync_mode   = sync;
  attr.hue              = hue;

  //set
  ret = rk_aiq_user_api2_acp_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set acp Attr failed!");
  printf("set acp hue: %d, done: %d\n\n", hue, attr.sync.done);

  return 0;
}

static int sample_acp_get_attrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  acp_attrib_t attr;
  memset(&attr,0,sizeof(acp_attrib_t));
  //get
  ret = rk_aiq_user_api2_acp_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "get acp Attr failed!");
  printf("get acp Attrib:\n\n");
  printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
  printf("\t brightness = %d, contrast = %d, saturation: %d, hue: %d, done: %d\n",
         attr.brightness, attr.contrast, attr.saturation, attr.hue, attr.sync.done);

  return 0;
}

#ifdef USE_NEWSTRUCT
static void sample_cp_tuningtool_test(const rk_aiq_sys_ctx_t* ctx)
{
    char *ret_str = NULL;

    printf(">>> start tuning tool test: op attrib get ...\n");

    std::string json_cp_status_str = " \n\
        [{ \n\
            \"op\":\"get\", \n\
            \"path\": \"/uapi/0/cp_uapi/info\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 0,\"bypass\": 3} \n\
        }]";

    rkaiq_uapi_unified_ctl(const_cast<rk_aiq_sys_ctx_t*>(ctx),
                           const_cast<char*>(json_cp_status_str.c_str()), &ret_str, RKAIQUAPI_OPMODE_GET);

    if (ret_str) {
        printf("cp status json str: %s\n", ret_str);
    }

    printf("  start tuning tool test: op attrib set ...\n");
    std::string json_cp_str = " \n\
        [{ \n\
            \"op\":\"replace\", \n\
            \"path\": \"/uapi/0/cp_uapi/attr\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 1,\"bypass\": 1} \n\
        }]";
    printf("cp json_cmd_str: %s\n", json_cp_str.c_str());
    ret_str = NULL;
    rkaiq_uapi_unified_ctl(const_cast<rk_aiq_sys_ctx_t*>(ctx),
                           const_cast<char*>(json_cp_str.c_str()), &ret_str, RKAIQUAPI_OPMODE_SET);

    // wait more than 2 frames
    usleep(90 * 1000);

    cp_status_t status;
    memset(&status, 0, sizeof(cp_status_t));

    rk_aiq_user_api2_cp_QueryStatus(ctx, &status);

    if (status.opMode != RK_AIQ_OP_MODE_MANUAL || status.en != 1 || status.bypass != 1) {
        printf("cp op set_attrib failed !\n");
        printf("cp status: opmode:%d(EXP:%d), en:%d(EXP:%d), bypass:%d(EXP:%d)\n",
               status.opMode, RK_AIQ_OP_MODE_MANUAL, status.en, 1, status.bypass, 1);
    } else {
        printf("cp op set_attrib success !\n");
    }

    printf(">>> tuning tool test done \n");
}

void get_auto_attr(cp_api_attrib_t* attr) {
    cp_param_auto_t* stAuto = &attr->stAuto;
    for (int i = 0;i < 13;i++) {
    }
}

void get_manual_attr(cp_api_attrib_t* attr) {
    cp_param_t* stMan = &attr->stMan;
}

int sample_cp_test(const rk_aiq_sys_ctx_t* ctx)
{
    // sample_cp_tuningtool_test(ctx);
    // get cur mode
    printf("+++++++ cp module test start ++++++++\n");

    cp_api_attrib_t attr;
    memset(&attr, 0, sizeof(attr));

    rk_aiq_user_api2_cp_GetAttrib(ctx, &attr);

    printf("cp attr: opmode:%d, en:%d, bypass:%d\n", attr.opMode, attr.en, attr.bypass);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update cp arrrib!\n");
        if (attr.opMode == RK_AIQ_OP_MODE_AUTO) {
            attr.opMode = RK_AIQ_OP_MODE_MANUAL;
            get_manual_attr(&attr);
        }
        else {
            get_auto_attr(&attr);
            attr.opMode = RK_AIQ_OP_MODE_AUTO;
        }
    }
    else {
        // reverse en
        printf("reverse cp en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_cp_SetAttrib(ctx, &attr);

    // wait more than 2 frames
    usleep(90 * 1000);

    cp_status_t status;
    memset(&status, 0, sizeof(cp_status_t));

    rk_aiq_user_api2_cp_QueryStatus(ctx, &status);

    printf("cp status: opmode:%d, en:%d, bypass:%d\n", status.opMode, status.en, status.bypass);

    if (status.opMode != attr.opMode || status.en != attr.en)
        printf("cp test failed\n");
    printf("-------- cp module test done --------\n");

    return 0;
}
#endif

XCamReturn sample_acp_module(const void *arg)
{
    int key = -1;
    CLEAR();
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    acp_attrib_t default_attr;
    memset(&default_attr, 0, sizeof(acp_attrib_t));
    rk_aiq_user_api2_acp_GetAttrib(ctx, &default_attr);

    sample_acp_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_acp_usage ();
                break;
            case '1':
                printf("test acp brightness iteratively in async mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_brightness(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_brightness(ctx, default_attr.brightness, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '2':
                printf("test acp brightness iteratively in sync mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_brightness(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_brightness(ctx, default_attr.brightness, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '3':
                printf("test cp contrast iteratively in async mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_contrast(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_contrast(ctx, default_attr.contrast, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '4':
                printf("test cp contrast iteratively in sync mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_contrast(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_contrast(ctx, default_attr.contrast, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '5':
                printf("test cp saturation iteratively in async mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_saturation(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_saturation(ctx, default_attr.saturation, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '6':
                printf("test cp saturation iteratively in sync mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_saturation(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_saturation(ctx, default_attr.saturation, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '7':
                printf("test cp saturation iteratively in async mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_hue(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_hue(ctx, default_attr.hue, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '8':
                printf("test cp saturation iteratively in sync mode...\n");
                for (int i = 0; i < 255; i++) {
                    sample_acp_set_hue(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_acp_set_hue(ctx, default_attr.hue, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '9':
                sample_acp_get_attrib(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case 'a':
                sample_acp_get_attrib(ctx, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case 'b':
                printf("test acp contrast iteratively ...\n");
                for (int i = 0; i < 255; i++) {
                    sample_set_acp_contrast(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_set_acp_contrast(ctx, default_attr.contrast);
                break;
            case 'c':
                printf("test acp brightness iteratively ...\n");
                for (int i = 0; i < 255; i++) {
                    sample_set_acp_brightness(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_set_acp_brightness(ctx, default_attr.brightness);
                break;
            case 'd':
                printf("test acp saturation iteratively ...\n");
                for (int i = 0; i < 255; i++) {
                    sample_set_acp_saturation(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_set_acp_saturation(ctx, default_attr.saturation);
                break;
            case 'e':
                printf("test acp hue iteratively ...\n");
                for (int i = 0; i < 255; i++) {
                    sample_set_acp_hue(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_set_acp_hue(ctx, default_attr.hue);
                break;
#ifdef USE_NEWSTRUCT
            case 'f':
                sample_cp_test(ctx);
                break;
#endif
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
