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

static void sample_aie_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t a) AIE:         Set effect mode.\n");
    printf("\t b) AIE:         Get effect mode.\n");
    printf("  Module API: \n");
    printf("\t 1) AIE:         Set effect mode  & Async .\n");
    printf("\t 2) AIE:         Set effect mode  & Sync .\n");
    printf("\t 3) AIE:         Get attrib       & Async .\n");
    printf("\t 4) AIE:         Get attrib       & Sync .\n");
    printf("\t 5) AIE:         newstruct test.\n");
    printf("\n");
    printf("\t h) AIE:         help.\n");
    printf("\t q) AIE:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_aie_info(const void *arg)
{
    printf ("enter AIE module test!\n");
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/
//rv1106 only support {0,1}
static int sample_aie_set_mode(const rk_aiq_sys_ctx_t* ctx, int mode,
                              rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  aie_attrib_t attr;
  memset(&attr, 0, sizeof(aie_attrib_t));

  ret = rk_aiq_user_api2_aie_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAttr failed in getting aie attrib!");

  attr.sync.sync_mode   = sync;
  attr.mode              = (rk_aiq_ie_effect_t)mode;

  //set
  ret = rk_aiq_user_api2_aie_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set aie Attr failed!");
  printf("set aie mode: %d, done: %d\n\n", mode, attr.sync.done);

  return 0;
}

static int sample_aie_get_attrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  aie_attrib_t attr;
  memset(&attr,0,sizeof(aie_attrib_t));
  //get
  ret = rk_aiq_user_api2_aie_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "get aie Attr failed!");
  printf("\t get aie mode: %d, done: %d\n", attr.mode, attr.sync.done);

  return 0;
}

/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/
// rv1106 only support {0,1}
static int sample_aie_set_effect_mode(const rk_aiq_sys_ctx_t* ctx, uint8_t mode)
{
    rk_aiq_uapi2_setColorMode(ctx, mode);
    return 0;
}

static int sample_aie_get_effect_mode(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int mode;
    rk_aiq_uapi2_getColorMode(ctx, &mode);
    switch (mode)
    {
    case 0:
        printf("Get AIE Mode is: NONE \n");
        break;
    case 1:
        printf("Get AIE Mode is: Black White \n");
        break;
    case 2:
        printf("Get AIE Mode is: NEGATIVE \n");
        break;
    case 3:
        printf("Get AIE Mode is: SEPIA \n");
        break;
    case 4:
        printf("Get AIE Mode is: EMBOSS \n");
        break;
    case 5:
        printf("Get AIE Mode is: SKETCH \n");
        break;
    case 6:
        printf("Get AIE Mode is: SHARPEN \n");
        break;
    default:
        break;
    }
    return 0;
}

#ifdef USE_NEWSTRUCT
static void sample_ie_tuningtool_test(const rk_aiq_sys_ctx_t* ctx)
{
    char *ret_str = NULL;

    printf(">>> start tuning tool test: op attrib get ...\n");

    std::string json_ie_status_str = " \n\
        [{ \n\
            \"op\":\"get\", \n\
            \"path\": \"/uapi/0/ie_uapi/info\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 0,\"bypass\": 3} \n\
        }]";

    rkaiq_uapi_unified_ctl(const_cast<rk_aiq_sys_ctx_t*>(ctx),
                           const_cast<char*>(json_ie_status_str.c_str()), &ret_str, RKAIQUAPI_OPMODE_GET);

    if (ret_str) {
        printf("ie status json str: %s\n", ret_str);
    }

    printf("  start tuning tool test: op attrib set ...\n");
    std::string json_ie_str = " \n\
        [{ \n\
            \"op\":\"replace\", \n\
            \"path\": \"/uapi/0/ie_uapi/attr\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 1,\"bypass\": 1} \n\
        }]";
    printf("ie json_cmd_str: %s\n", json_ie_str.c_str());
    ret_str = NULL;
    rkaiq_uapi_unified_ctl(const_cast<rk_aiq_sys_ctx_t*>(ctx),
                           const_cast<char*>(json_ie_str.c_str()), &ret_str, RKAIQUAPI_OPMODE_SET);

    // wait more than 2 frames
    usleep(90 * 1000);

    ie_status_t status;
    memset(&status, 0, sizeof(ie_status_t));

    rk_aiq_user_api2_ie_QueryStatus(ctx, &status);

    if (status.opMode != RK_AIQ_OP_MODE_MANUAL || status.en != 1 || status.bypass != 1) {
        printf("ie op set_attrib failed !\n");
        printf("ie status: opmode:%d(EXP:%d), en:%d(EXP:%d), bypass:%d(EXP:%d)\n",
               status.opMode, RK_AIQ_OP_MODE_MANUAL, status.en, 1, status.bypass, 1);
    } else {
        printf("ie op set_attrib success !\n");
    }

    printf(">>> tuning tool test done \n");
}

void get_auto_attr(ie_api_attrib_t* attr) {
    ie_param_auto_t* stAuto = &attr->stAuto;
    for (int i = 0;i < 13;i++) {
    }
}

void get_manual_attr(ie_api_attrib_t* attr) {
    ie_param_t* stMan = &attr->stMan;
}

void sample_new_ie(const rk_aiq_sys_ctx_t* ctx) {
    // sample_ie_tuningtool_test(ctx);

    ie_api_attrib_t attr;
    ie_status_t status;
    rk_aiq_user_api2_ie_GetAttrib(ctx, &attr);
    printf("\t attr.opMode:%d attr.en:%d\n\n",
            attr.opMode, attr.en);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update ie arrrib!\n");
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
        printf("reverse ie en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_ie_SetAttrib(ctx, &attr);

    rk_aiq_user_api2_ie_QueryStatus(ctx, &status);
    printf("\t status.opMode:%d status.en:%d\n\n",
            status.opMode, status.en);
}
#endif

XCamReturn sample_aie_module(const void *arg)
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

    aie_attrib_t default_attr;
    memset(&default_attr, 0, sizeof(aie_attrib_t));
    rk_aiq_user_api2_aie_GetAttrib(ctx, &default_attr);

    sample_aie_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_aie_usage ();
                break;
            case '1':
                printf("test aie effect mode iteratively in async mode...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_mode(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_aie_set_mode(ctx, default_attr.mode, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '2':
                printf("test aie effect mode iteratively in sync mode...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_mode(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_aie_set_mode(ctx, default_attr.mode, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '3':
                sample_aie_get_attrib(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '4':
                sample_aie_get_attrib(ctx, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '5':
                #ifdef USE_NEWSTRUCT
                sample_new_ie(ctx);
                #endif
                break;
            case 'a':
                printf("test aie effect mode iteratively...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_effect_mode(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");
                sample_aie_set_effect_mode(ctx, default_attr.mode);
                break;
            case 'b':
                sample_aie_get_effect_mode(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
