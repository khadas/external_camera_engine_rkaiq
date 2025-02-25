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
//#include <string>
static void sample_adehaze_usage()
{
    printf("Usage : \n");
    // printf("\t 0) ADEHAZE:         test rk_aiq_user_api2_adehaze_v10_setSwAttrib
    // DEHAZE_API_MANUAL Sync.\n"); printf("\t 1) ADEHAZE:         test
    // rk_aiq_user_api2_adehaze_v10_setSwAttrib DEHAZE_API_MANUAL Async.\n");
    printf(
        "\t 0) ADEHAZE:         test rk_aiq_user_api2_adehaze_v11_setSwAttrib DEHAZE_API_MANUAL "
        "Sync.\n");
    printf(
        "\t 1) ADEHAZE:         test rk_aiq_user_api2_adehaze_v11_setSwAttrib DEHAZE_API_MANUAL "
        "Async.\n");
    printf("\t 2) ADEHAZE:         test rk_aiq_user_api2_adehaze_v11_getSwAttrib.\n");
    printf(
        "\t 3) ADEHAZE:         test rk_aiq_user_api2_adehaze_v12_setSwAttrib DEHAZE_API_MANUAL "
        "Sync.\n");
    printf(
        "\t 4) ADEHAZE:         test rk_aiq_user_api2_adehaze_v12_setSwAttrib DEHAZE_API_MANUAL "
        "Async.\n");
    printf("\t 5) ADEHAZE:         test rk_aiq_user_api2_adehaze_v12_getSwAttrib.\n");
    printf(
        "\t 6) ADEHAZE:         test rk_aiq_user_api2_adehaze_setSwAttrib DEHAZE_API_MANUAL "
        "Sync.\n");
    printf(
        "\t 7) ADEHAZE:         test rk_aiq_user_api2_adehaze_setSwAttrib DEHAZE_API_MANUAL "
        "Async.\n");
    printf("\t 8) ADEHAZE:         test rk_aiq_user_api2_adehaze_getSwAttrib.\n");
    printf("\t 9) ADEHAZE:         test rk_aiq_uapi2_setDehazeModuleEnable.\n");
    printf("\t a) ADEHAZE:         test rk_aiq_uapi2_setDehazeEnable.\n");
    printf("\t b) ADEHAZE:         test rk_aiq_uapi2_getMDehazeStrth.\n");
    printf("\t c) ADEHAZE:         test rk_aiq_uapi2_setMDehazeStrth.\n");
    printf("\t d) ADEHAZE:         test rk_aiq_uapi2_setEnhanceEnable.\n");
    printf("\t e) ADEHAZE:         test rk_aiq_uapi2_getMEnhanceStrth.\n");
    printf("\t f) ADEHAZE:         test rk_aiq_uapi2_setMEnhanceStrth.\n");
    printf("\t g) ADEHAZE:         test rk_aiq_uapi2_getMEnhanceChromeStrth.\n");
    printf("\t h) ADEHAZE:         test rk_aiq_uapi2_setMEnhanceChromeStrth.\n");
    printf("\t l) ADEHAZE:         sample_new_dehaze.\n");
    printf("\t n) HISTEQ:         sample_new_histeq.\n");
    printf("\t m) ENHANCE:         sample_new_enh.\n");
    printf("\t q) ADEHAZE:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_adehaze_info(const void *arg)
{
    printf ("enter ADEHAZE modult test!\n");
}

#if USE_NEWSTRUCT && !defined(ISP_HW_V33)

static void sample_dehaze_tuningtool_test(const rk_aiq_sys_ctx_t* ctx)
{
    char *ret_str = NULL;

    printf(">>> start tuning tool test: op attrib get ...\n");

    const char* json_dehaze_status_str = " \n\
        [{ \n\
            \"op\":\"get\", \n\
            \"path\": \"/uapi/0/dehaze_uapi/info\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 0,\"bypass\": 3} \n\
        }]";

    rkaiq_uapi_unified_ctl((rk_aiq_sys_ctx_t*)(ctx),
                           json_dehaze_status_str, &ret_str, RKAIQUAPI_OPMODE_GET);

    if (ret_str) {
        printf("dehaze status json str: %s\n", ret_str);
    }

    printf("  start tuning tool test: op attrib set ...\n");
    const char* json_dehaze_str = " \n\
        [{ \n\
            \"op\":\"replace\", \n\
            \"path\": \"/uapi/0/dehaze_uapi/attr\", \n\
            \"value\": \n\
            { \"opMode\": \"RK_AIQ_OP_MODE_MANUAL\", \"en\": 1,\"bypass\": 1} \n\
        }]";
    printf("dehaze json_cmd_str: %s\n", json_dehaze_str);
    ret_str = NULL;
    rkaiq_uapi_unified_ctl((rk_aiq_sys_ctx_t*)(ctx),
                           json_dehaze_str, &ret_str, RKAIQUAPI_OPMODE_SET);

    // wait more than 2 frames
    usleep(90 * 1000);

    dehaze_status_t status;
    memset(&status, 0, sizeof(dehaze_status_t));

    rk_aiq_user_api2_dehaze_QueryStatus(ctx, &status);

    if (status.opMode != RK_AIQ_OP_MODE_MANUAL || status.en != 1 || status.bypass != 1) {
        printf("dehaze op set_attrib failed !\n");
        printf("dehaze status: opmode:%d(EXP:%d), en:%d(EXP:%d), bypass:%d(EXP:%d)\n",
               status.opMode, RK_AIQ_OP_MODE_MANUAL, status.en, 1, status.bypass, 1);
    } else {
        printf("dehaze op set_attrib success !\n");
    }

    printf(">>> tuning tool test done \n");
}

static void get_auto_attr(dehaze_api_attrib_t* attr) {
    dehaze_param_auto_t* stAuto = &attr->stAuto;
    for (int i = 0;i < 13;i++) {
    }
}

static void get_manual_attr(dehaze_api_attrib_t* attr) {
    dehaze_param_t* stMan = &attr->stMan;
}

void sample_new_dehaze(const rk_aiq_sys_ctx_t* ctx)
{
    // sample_dehaze_tuningtool_test(ctx);
    dehaze_api_attrib_t attr;
    dehaze_status_t status;
    rk_aiq_user_api2_dehaze_GetAttrib(ctx, &attr);
    printf("\t attr.opMode:%d attr.en:%d\n\n",
            attr.opMode, attr.en);

    printf("\t attr.opMode:%d attr.en:%d\n\n",
            attr.opMode, attr.en);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update dehaze arrrib!\n");
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
        printf("reverse dehaze en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_dehaze_SetAttrib(ctx, &attr);

    rk_aiq_user_api2_dehaze_QueryStatus(ctx, &status);
    printf("\t status.opMode:%d status.en:%d\n\n",
            status.opMode, status.en);
}
#endif

#if USE_NEWSTRUCT && defined(RKAIQ_HAVE_HISTEQ)
void sample_new_histeq(const rk_aiq_sys_ctx_t* ctx)
{
    histeq_api_attrib_t attr;
    histeq_status_t status;
    rk_aiq_user_api2_histeq_GetAttrib(ctx, &attr);

    printf("\t attr.opMode:%d attr.en:%d\n\n",
            attr.opMode, attr.en);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update histeq arrrib!\n");
        if (attr.opMode == RK_AIQ_OP_MODE_AUTO) {
            attr.opMode = RK_AIQ_OP_MODE_MANUAL;
        }
        else {
            attr.opMode = RK_AIQ_OP_MODE_AUTO;
        }
    }
    else {
        // reverse en
        printf("reverse histeq en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_histeq_SetAttrib(ctx, &attr);

    // wait more than 2 frames
    usleep(90 * 1000);

    rk_aiq_user_api2_histeq_QueryStatus(ctx, &status);
    printf("\t status.opMode:%d status.en:%d\n\n",
            status.opMode, status.en);
}
#endif

#if USE_NEWSTRUCT && defined(RKAIQ_HAVE_ENHANCE)
void sample_new_enh(const rk_aiq_sys_ctx_t* ctx)
{
    enh_api_attrib_t attr;
    enh_status_t status;
    rk_aiq_user_api2_enh_GetAttrib(ctx, &attr);

    printf("\t attr.opMode:%d attr.en:%d\n\n",
            attr.opMode, attr.en);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update enh arrrib!\n");
        if (attr.opMode == RK_AIQ_OP_MODE_AUTO) {
            attr.opMode = RK_AIQ_OP_MODE_MANUAL;
        }
        else {
            attr.opMode = RK_AIQ_OP_MODE_AUTO;
        }
    }
    else {
        // reverse en
        printf("reverse enh en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_enh_SetAttrib(ctx, &attr);
    // wait more than 2 frames
    usleep(90 * 1000);

    rk_aiq_user_api2_enh_QueryStatus(ctx, &status);
    printf("\t status.opMode:%d status.en:%d\n\n",
            status.opMode, status.en);
}
#endif

XCamReturn sample_adehaze_module(const void* arg)
{
    int key = -1;
    CLEAR();

    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx ;
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    do {
        sample_adehaze_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        adehaze_sw_v11_t attr_v11;
        memset(&attr_v11, 0, sizeof(adehaze_sw_v11_t));
        adehaze_sw_v12_t attr_v12;
        memset(&attr_v12, 0, sizeof(adehaze_sw_v12_t));
        adehaze_sw_V2_t attr_v2;
        memset(&attr_v2, 0, sizeof(adehaze_sw_V2_t));

        switch (key)
        {
        case '0': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_v11_setSwAttrib DEHAZE_API_MANUAL "
                "Sync\n\n");
            attr_v11.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_DEFAULT;
            attr_v11.sync.done                                          = false;
            attr_v11.mode                                               = DEHAZE_API_MANUAL;
            attr_v11.stManual.Enable                                    = true;
            attr_v11.stManual.cfg_alpha                                 = 1.0f;
            attr_v11.stManual.dehaze_setting.en                         = false;
            attr_v11.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v11.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v11.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v11.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v11.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v11.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v11.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v11.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v11.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_air         = 211.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v11.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v11.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v11.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v11.stManual.enhance_setting.en                         = true;
            attr_v11.stManual.enhance_setting.EnhanceData.enhance_value  = 1.0f;
            attr_v11.stManual.enhance_setting.EnhanceData.enhance_chroma = 1.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[0]           = 0.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[1]           = 64.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[2]           = 128.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[3]           = 192.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[4]           = 256.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[5]           = 320.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[6]           = 384.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[7]           = 448.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[8]           = 512.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[9]           = 576.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[10]          = 640.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[11]          = 704.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[12]          = 768.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[13]          = 832.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[14]          = 896.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[15]          = 960.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[16]          = 1023.0f;

            attr_v11.stManual.hist_setting.en                   = false;
            attr_v11.stManual.hist_setting.hist_para_en         = true;
            attr_v11.stManual.hist_setting.HistData.hist_gratio = 2.0f;
            attr_v11.stManual.hist_setting.HistData.hist_th_off = 64.0f;
            attr_v11.stManual.hist_setting.HistData.hist_k      = 2.0f;
            attr_v11.stManual.hist_setting.HistData.hist_min    = 0.015f;
            attr_v11.stManual.hist_setting.HistData.hist_scale  = 0.09f;
            attr_v11.stManual.hist_setting.HistData.cfg_gratio  = 2.0f;

            rk_aiq_user_api2_adehaze_v11_setSwAttrib(ctx, &attr_v11);
            break;
        }
        case '1': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_v11_setSwAttrib DEHAZE_API_MANUAL "
                "Async\n\n");
            attr_v11.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_ASYNC;
            attr_v11.sync.done                                          = false;
            attr_v11.mode                                               = DEHAZE_API_MANUAL;
            attr_v11.stManual.Enable                                    = true;
            attr_v11.stManual.cfg_alpha                                 = 0.0f;
            attr_v11.stManual.dehaze_setting.en                         = false;
            attr_v11.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v11.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v11.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v11.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v11.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v11.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v11.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v11.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v11.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v11.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_air         = 211.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v11.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v11.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v11.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v11.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v11.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v11.stManual.enhance_setting.en                         = true;
            attr_v11.stManual.enhance_setting.EnhanceData.enhance_value  = 2.0f;
            attr_v11.stManual.enhance_setting.EnhanceData.enhance_chroma = 2.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[0]           = 0.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[1]           = 64.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[2]           = 128.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[3]           = 192.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[4]           = 256.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[5]           = 320.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[6]           = 384.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[7]           = 448.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[8]           = 512.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[9]           = 576.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[10]          = 640.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[11]          = 704.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[12]          = 768.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[13]          = 832.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[14]          = 896.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[15]          = 960.0f;
            attr_v11.stManual.enhance_setting.enhance_curve[16]          = 1023.0f;

            attr_v11.stManual.hist_setting.en                   = false;
            attr_v11.stManual.hist_setting.hist_para_en         = true;
            attr_v11.stManual.hist_setting.HistData.hist_gratio = 2.0f;
            attr_v11.stManual.hist_setting.HistData.hist_th_off = 64.0f;
            attr_v11.stManual.hist_setting.HistData.hist_k      = 2.0f;
            attr_v11.stManual.hist_setting.HistData.hist_min    = 0.015f;
            attr_v11.stManual.hist_setting.HistData.hist_scale  = 0.09f;
            attr_v11.stManual.hist_setting.HistData.cfg_gratio  = 2.0f;

            rk_aiq_user_api2_adehaze_v11_setSwAttrib(ctx, &attr_v11);
            break;
        }
        case '2': {
            printf("\t ADEHAZE test rk_aiq_user_api2_adehaze_v11_getSwAttrib\n\n");
            rk_aiq_user_api2_adehaze_v11_getSwAttrib(ctx, &attr_v11);
            printf("\t sync = %d, done = %d\n", attr_v11.sync.sync_mode, attr_v11.sync.done);
            printf("\t mode: %d\n\n", attr_v11.mode);
            printf("\t stManual Enable: %d cfg_alpha:%f\n\n", attr_v11.stManual.Enable,
                   attr_v11.stManual.cfg_alpha);
            printf("\t MDehazeStrth: %d MEnhanceStrth:%d MEnhanceChromeStrth:%d\n\n",
                   attr_v11.Info.MDehazeStrth, attr_v11.Info.MEnhanceStrth,
                   attr_v11.Info.MEnhanceChromeStrth);
            break;
        }
        case '3': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_v12_setSwAttrib DEHAZE_API_MANUAL "
                "Sync\n\n");
            attr_v12.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_DEFAULT;
            attr_v12.sync.done                                          = false;
            attr_v12.mode                                               = DEHAZE_API_MANUAL;
            attr_v12.stManual.Enable                                    = true;
            attr_v12.stManual.cfg_alpha                                 = 1.0f;
            attr_v12.stManual.dehaze_setting.en                         = false;
            attr_v12.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v12.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v12.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v12.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v12.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v12.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v12.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v12.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v12.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_air         = 212.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v12.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v12.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v12.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v12.stManual.enhance_setting.en                         = true;
            attr_v12.stManual.enhance_setting.color_deviate_en           = false;
            attr_v12.stManual.enhance_setting.enh_luma_en                = false;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_value     = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_chroma    = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[0]  = 0.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[1]  = 64.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[2]  = 128.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[3]  = 192.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[4]  = 256.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[5]  = 320.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[6]  = 384.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[7]  = 448.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[8]  = 512.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[9]  = 576.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[10] = 640.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[11] = 704.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[12] = 768.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[13] = 832.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[14] = 896.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[15] = 960.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[16] = 1023.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[0]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[1]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[2]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[3]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[4]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[5]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[6]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[7]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[8]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[9]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[10]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[11]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[12]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[13]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[14]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[15]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[16]      = 1.0f;

            attr_v12.stManual.hist_setting.en                   = false;
            attr_v12.stManual.hist_setting.hist_para_en         = true;
            attr_v12.stManual.hist_setting.hist_wr.mode                     = HIST_WR_AUTO;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[0]  = 0;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[1]  = 64;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[2]  = 128;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[3]  = 192;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[4]  = 256;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[5]  = 320;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[6]  = 384;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[7]  = 448;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[8]  = 512;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[9]  = 576;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[10] = 640;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[11] = 704;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[12] = 768;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[13] = 832;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[14] = 896;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[15] = 960;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[16] = 1023;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[0]  = 0;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[1]  = 64;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[2]  = 128;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[3]  = 192;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[4]  = 256;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[5]  = 320;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[6]  = 384;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[7]  = 448;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[8]  = 512;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[9]  = 576;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[10] = 640;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[11] = 704;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[12] = 768;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[13] = 832;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[14] = 896;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[15] = 960;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[16] = 1023;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.clim0     = 1.0f;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.clim1     = 1.0f;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.dark_th   = 0.0f;
            attr_v12.stManual.hist_setting.HistData.hist_gratio             = 2.0f;
            attr_v12.stManual.hist_setting.HistData.hist_th_off             = 64.0f;
            attr_v12.stManual.hist_setting.HistData.hist_k                  = 2.0f;
            attr_v12.stManual.hist_setting.HistData.hist_min                = 0.015f;
            attr_v12.stManual.hist_setting.HistData.hist_scale              = 0.09f;
            attr_v12.stManual.hist_setting.HistData.cfg_gratio              = 2.0f;

            rk_aiq_user_api2_adehaze_v12_setSwAttrib(ctx, &attr_v12);
            break;
        }
        case '4': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_v12_setSwAttrib DEHAZE_API_MANUAL "
                "Async\n\n");
            attr_v12.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_ASYNC;
            attr_v12.sync.done                                          = false;
            attr_v12.mode                                               = DEHAZE_API_MANUAL;
            attr_v12.stManual.Enable                                    = false;
            attr_v12.stManual.cfg_alpha                                 = 0.0f;
            attr_v12.stManual.dehaze_setting.en                         = false;
            attr_v12.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v12.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v12.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v12.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v12.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v12.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v12.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v12.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v12.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v12.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_air         = 212.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v12.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v12.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v12.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v12.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v12.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v12.stManual.enhance_setting.en                         = true;
            attr_v12.stManual.enhance_setting.color_deviate_en           = false;
            attr_v12.stManual.enhance_setting.enh_luma_en                = false;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_value     = 2.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_chroma    = 2.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[0]  = 0.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[1]  = 64.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[2]  = 128.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[3]  = 192.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[4]  = 256.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[5]  = 320.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[6]  = 384.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[7]  = 448.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[8]  = 512.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[9]  = 576.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[10] = 640.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[11] = 704.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[12] = 768.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[13] = 832.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[14] = 896.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[15] = 960.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enhance_curve[16] = 1023;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[0]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[1]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[2]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[3]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[4]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[5]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[6]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[7]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[8]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[9]       = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[10]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[11]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[12]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[13]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[14]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[15]      = 1.0f;
            attr_v12.stManual.enhance_setting.EnhanceData.enh_luma[16]      = 1.0f;

            attr_v12.stManual.hist_setting.en                   = false;
            attr_v12.stManual.hist_setting.hist_para_en         = true;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[0]  = 0;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[1]  = 64;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[2]  = 128;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[3]  = 192;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[4]  = 256;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[5]  = 320;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[6]  = 384;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[7]  = 448;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[8]  = 512;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[9]  = 576;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[10] = 640;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[11] = 704;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[12] = 768;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[13] = 832;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[14] = 896;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[15] = 960;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_x[16] = 1023;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[0]  = 0;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[1]  = 64;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[2]  = 128;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[3]  = 192;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[4]  = 256;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[5]  = 320;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[6]  = 384;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[7]  = 448;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[8]  = 512;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[9]  = 576;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[10] = 640;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[11] = 704;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[12] = 768;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[13] = 832;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[14] = 896;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[15] = 960;
            attr_v12.stManual.hist_setting.hist_wr.manual_curve.curve_y[16] = 1023;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.clim0     = 1.0f;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.clim1     = 1.0f;
            attr_v12.stManual.hist_setting.hist_wr.semiauto_curve.dark_th   = 0.0f;
            attr_v12.stManual.hist_setting.HistData.hist_gratio             = 2.0f;
            attr_v12.stManual.hist_setting.HistData.hist_th_off             = 64.0f;
            attr_v12.stManual.hist_setting.HistData.hist_k                  = 2.0f;
            attr_v12.stManual.hist_setting.HistData.hist_min                = 0.015f;
            attr_v12.stManual.hist_setting.HistData.hist_scale              = 0.09f;
            attr_v12.stManual.hist_setting.HistData.cfg_gratio              = 2.0f;

            rk_aiq_user_api2_adehaze_v12_setSwAttrib(ctx, &attr_v12);
            break;
        }
        case '5': {
            printf("\t ADEHAZE test rk_aiq_user_api2_adehaze_v12_getSwAttrib\n\n");
            rk_aiq_user_api2_adehaze_v12_getSwAttrib(ctx, &attr_v12);
            printf("\t sync = %d, done = %d\n", attr_v12.sync.sync_mode, attr_v12.sync.done);
            printf("\t mode: %d\n\n", attr_v12.mode);
            printf("\t stManual Enable: %d cfg_alpha:%f\n\n", attr_v12.stManual.Enable,
                   attr_v12.stManual.cfg_alpha);
            printf("\t MDehazeStrth: %d MEnhanceStrth:%d MEnhanceChromeStrth:%d\n\n",
                   attr_v12.Info.MDehazeStrth, attr_v12.Info.MEnhanceStrth,
                   attr_v12.Info.MEnhanceChromeStrth);
            break;
        }
        case '6': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_setSwAttrib DEHAZE_API_MANUAL "
                "Sync\n\n");
            attr_v2.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_DEFAULT;
            attr_v2.sync.done                                          = false;
            attr_v2.mode                                               = DEHAZE_API_MANUAL;
            attr_v2.stManual.Enable                                    = true;
            attr_v2.stManual.cfg_alpha                                 = 1.0f;
            attr_v2.stManual.dehaze_setting.en                         = false;
            attr_v2.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v2.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v2.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v2.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v2.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v2.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v2.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v2.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v2.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_air         = 211.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v2.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v2.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v2.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v2.stManual.enhance_setting.en                         = true;
            attr_v2.stManual.enhance_setting.EnhanceData.enhance_value  = 1.0f;
            attr_v2.stManual.enhance_setting.EnhanceData.enhance_chroma = 1.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[0]           = 0.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[1]           = 64.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[2]           = 128.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[3]           = 192.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[4]           = 256.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[5]           = 320.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[6]           = 384.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[7]           = 448.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[8]           = 512.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[9]           = 576.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[10]          = 640.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[11]          = 704.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[12]          = 768.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[13]          = 832.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[14]          = 896.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[15]          = 960.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[16]          = 1023.0f;

            attr_v2.stManual.hist_setting.en                   = false;
            attr_v2.stManual.hist_setting.hist_para_en         = true;
            attr_v2.stManual.hist_setting.HistData.hist_gratio = 2.0f;
            attr_v2.stManual.hist_setting.HistData.hist_th_off = 64.0f;
            attr_v2.stManual.hist_setting.HistData.hist_k      = 2.0f;
            attr_v2.stManual.hist_setting.HistData.hist_min    = 0.015f;
            attr_v2.stManual.hist_setting.HistData.hist_scale  = 0.09f;
            attr_v2.stManual.hist_setting.HistData.cfg_gratio  = 2.0f;

            rk_aiq_user_api2_adehaze_setSwAttrib(ctx, attr_v2);
            break;
        }
        case '7': {
            printf(
                "\t ADEHAZE test rk_aiq_user_api2_adehaze_setSwAttrib DEHAZE_API_MANUAL "
                "Async\n\n");
            attr_v2.sync.sync_mode                                     = RK_AIQ_UAPI_MODE_ASYNC;
            attr_v2.sync.done                                          = false;
            attr_v2.mode                                               = DEHAZE_API_MANUAL;
            attr_v2.stManual.Enable                                    = true;
            attr_v2.stManual.cfg_alpha                                 = 0.0f;
            attr_v2.stManual.dehaze_setting.en                         = false;
            attr_v2.stManual.dehaze_setting.air_lc_en                  = true;
            attr_v2.stManual.dehaze_setting.stab_fnum                  = 8.0f;
            attr_v2.stManual.dehaze_setting.sigma                      = 6.0f;
            attr_v2.stManual.dehaze_setting.wt_sigma                   = 8.0f;
            attr_v2.stManual.dehaze_setting.air_sigma                  = 120.0f;
            attr_v2.stManual.dehaze_setting.tmax_sigma                 = 0.01f;
            attr_v2.stManual.dehaze_setting.pre_wet                    = 0.01f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_min_th       = 64.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_max_th       = 192.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.yhist_th        = 249.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.yblk_th         = 0.002f;
            attr_v2.stManual.dehaze_setting.DehazeData.dark_th         = 250.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bright_min      = 180.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bright_max      = 240.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.wt_max          = 0.9f;
            attr_v2.stManual.dehaze_setting.DehazeData.air_min         = 200.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.air_max         = 250.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_base       = 125.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_off        = 0.1f;
            attr_v2.stManual.dehaze_setting.DehazeData.tmax_max        = 0.8f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_wt          = 0.8f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_air         = 211.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.cfg_tmax        = 0.2f;
            attr_v2.stManual.dehaze_setting.DehazeData.dc_weitcur      = 1.0f;
            attr_v2.stManual.dehaze_setting.DehazeData.bf_weight       = 0.5f;
            attr_v2.stManual.dehaze_setting.DehazeData.range_sigma     = 0.14f;
            attr_v2.stManual.dehaze_setting.DehazeData.space_sigma_pre = 0.14f;
            attr_v2.stManual.dehaze_setting.DehazeData.space_sigma_cur = 0.14f;

            attr_v2.stManual.enhance_setting.en                         = true;
            attr_v2.stManual.enhance_setting.EnhanceData.enhance_value  = 2.0f;
            attr_v2.stManual.enhance_setting.EnhanceData.enhance_chroma = 2.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[0]           = 0.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[1]           = 64.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[2]           = 128.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[3]           = 192.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[4]           = 256.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[5]           = 320.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[6]           = 384.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[7]           = 448.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[8]           = 512.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[9]           = 576.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[10]          = 640.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[11]          = 704.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[12]          = 768.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[13]          = 832.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[14]          = 896.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[15]          = 960.0f;
            attr_v2.stManual.enhance_setting.enhance_curve[16]          = 1023.0f;

            attr_v2.stManual.hist_setting.en                   = false;
            attr_v2.stManual.hist_setting.hist_para_en         = true;
            attr_v2.stManual.hist_setting.HistData.hist_gratio = 2.0f;
            attr_v2.stManual.hist_setting.HistData.hist_th_off = 64.0f;
            attr_v2.stManual.hist_setting.HistData.hist_k      = 2.0f;
            attr_v2.stManual.hist_setting.HistData.hist_min    = 0.015f;
            attr_v2.stManual.hist_setting.HistData.hist_scale  = 0.09f;
            attr_v2.stManual.hist_setting.HistData.cfg_gratio  = 2.0f;

            rk_aiq_user_api2_adehaze_setSwAttrib(ctx, attr_v2);
            break;
        }
        case '8': {
            printf("\t ADEHAZE test rk_aiq_user_api2_adehaze_getSwAttrib\n\n");
            rk_aiq_user_api2_adehaze_getSwAttrib(ctx, &attr_v2);
            printf("\t sync = %d, done = %d\n", attr_v2.sync.sync_mode, attr_v2.sync.done);
            printf("\t mode: %d\n\n", attr_v2.mode);
            printf("\t stManual Enable: %d cfg_alpha:%f\n\n", attr_v2.stManual.Enable,
                   attr_v2.stManual.cfg_alpha);
            printf("\t MDehazeStrth: %d MEnhanceStrth:%d \n\n", attr_v2.stDehazeManu.level,
                   attr_v2.stEnhanceManu.level);
            break;
        }
        case '9': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setDehazeModuleEnable\n\n");
            rk_aiq_uapi2_setDehazeModuleEnable(ctx, false);
            break;
        }
        case 'a': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setDehazeEnable\n\n");
            rk_aiq_uapi2_setDehazeEnable(ctx, false);
            break;
        }
        case 'b': {
            printf("\t ADEHAZE test rk_aiq_uapi2_getMDehazeStrth\n\n");
            unsigned int level = 60;
            rk_aiq_uapi2_getMDehazeStrth(ctx, &level);
            printf("\t rk_aiq_uapi2_getMDehazeStrth level: %u\n\n", level);
            break;
        }
        case 'c': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setMDehazeStrth\n\n");
            rk_aiq_uapi2_setDehazeModuleEnable(ctx, true);
            rk_aiq_uapi2_setDehazeEnable(ctx, true);
            unsigned int level = 70;
            rk_aiq_uapi2_setMDehazeStrth(ctx, level);
            printf("\t rk_aiq_uapi2_setMDehazeStrth level: %u\n\n", level);
            break;
        }
        case 'd': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setEnhanceEnable\n\n");
            rk_aiq_uapi2_setEnhanceEnable(ctx, false);
            break;
        }
        case 'e': {
            printf("\t ADEHAZE test rk_aiq_uapi2_getMEnhanceStrth\n\n");
            unsigned int level = 60;
            rk_aiq_uapi2_getMEnhanceStrth(ctx, &level);
            printf("\t rk_aiq_uapi2_getMEnhanceStrth level: %u\n\n", level);
            break;
        }
        case 'f': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setMEnhanceStrth\n\n");
            rk_aiq_uapi2_setDehazeModuleEnable(ctx, true);
            rk_aiq_uapi2_setEnhanceEnable(ctx, true);
            unsigned int level = 70;
            rk_aiq_uapi2_setMEnhanceStrth(ctx, level);
            printf("\t rk_aiq_uapi2_setMEnhanceStrth level: %u\n\n", level);
            break;
        }
        case 'g': {
            printf("\t ADEHAZE test rk_aiq_uapi2_getMEnhanceChromeStrth\n\n");
            unsigned int level = 60;
            rk_aiq_uapi2_getMEnhanceChromeStrth(ctx, &level);
            printf("\t rk_aiq_uapi2_getMEnhanceChromeStrth level: %u\n\n", level);
            break;
        }
        case 'h': {
            printf("\t ADEHAZE test rk_aiq_uapi2_setMEnhanceChromeStrth\n\n");
            rk_aiq_uapi2_setDehazeModuleEnable(ctx, true);
            rk_aiq_uapi2_setEnhanceEnable(ctx, true);
            unsigned int level = 70;
            rk_aiq_uapi2_setMEnhanceChromeStrth(ctx, level);
            printf("\t rk_aiq_uapi2_setMEnhanceChromeStrth level: %u\n\n", level);
            break;
        }
#if USE_NEWSTRUCT && !defined(ISP_HW_V33)
        case 'l': {
            printf("\t sample_new_dehaze\n\n");
            sample_new_dehaze(ctx);
            break;
        }
#endif
#if USE_NEWSTRUCT && defined(RKAIQ_HAVE_HISTEQ)
        case 'n': {
            printf("\t sample_new_histeq\n\n");
            sample_new_histeq(ctx);
            break;
        }
#endif
#if USE_NEWSTRUCT && defined(RKAIQ_HAVE_ENHANCE)
        case 'm': {
            printf("\t sample_new_enh\n\n");
            sample_new_enh(ctx);
            break;
        }       
#endif
        default:
            break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
