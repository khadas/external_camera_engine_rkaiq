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

static void sample_acnr_usage()
{
    printf("Usage : \n");
    printf("\t 0) ACNR:         get acnr attri on sync mode.\n");
    printf("\t 1) ACNR:         get acnr strength on sync mode.\n");
    printf("\t 2) ACNR:         set acnr attri auto on sync mode.\n");
    printf("\t 3) ACNR:         set acnr attri manual on sync mode.\n");
    printf("\t 4) ACNR:         set acnr attri reg value on sync mode.\n");
    printf("\t 5) ACNR:         set acnr strength max value 1.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 6) ACNR:         set acnr strength min value 0.0 on sync mode, only on auto mode has effect.\n");
    printf("\t 7) ACNR:         set acnr strength med value 0.5 on sync mode, only on auto mode has effect.\n");
    printf("\t 8) ACNR:         set acnr attri to default vaule on sync mode.\n");
    printf("\t a) ACNR:         get acnr attri on async mode.\n");
    printf("\t b) ACNR:         get acnr strength on async mode.\n");
    printf("\t c) ACNR:         set acnr attri auto on async mode.\n");
    printf("\t d) ACNR:         set acnr attri manual on async mode.\n");
    printf("\t e) ACNR:         set acnr attri reg value on async mode.\n");
    printf("\t f) ACNR:         set acnr strength max value 1.0 on async mode, only on auto mode has effect.\n");
    printf("\t g) ACNR:         set acnr strength min value 0.0 on async mode, only on auto mode has effect.\n");
    printf("\t h) ACNR:         set acnr strength med value 0.5 on async mode, only on auto mode has effect.\n");
    printf("\t i) ACNR:         set acnr attri to default vaule on async mode.\n");
    printf("\t j) ACNR:         sample_cnr_reverseEn.\n");
    printf("\t q) ACNR:         press key q or Q to quit.\n");

}

void sample_print_acnr_info(const void *arg)
{
    printf ("enter ACNR modult test!\n");
}

XCamReturn sample_acnr_getAtrr_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v2_t cnrV2_attr;
    cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &cnrV2_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, cnrV2_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_getAtrr_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v30_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &cnr_attr);
    printf("get acnr v30 attri ret:%d done:%d\n\n", ret, cnr_attr.sync.done);
    return ret;
}


XCamReturn sample_acnr_getAtrr_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v31_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &cnr_attr);
    printf("get acnr v31 attri ret:%d done:%d\n\n", ret, cnr_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_getStrength_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v2_t cnrV2_Strenght;
    cnrV2_Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetStrength(ctx, &cnrV2_Strenght);
    printf("get acnr v2 attri ret:%d strength:%f done:%d \n\n",
           ret, cnrV2_Strenght.percent, cnrV2_Strenght.sync.done);
    return ret;
}

XCamReturn sample_acnr_getStrength_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v30_t cnr_Strength;
    cnr_Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetStrength(ctx, &cnr_Strength);
    printf("get acnr v30 attri ret:%d strength:%f done:%d \n\n",
           ret, cnr_Strength.percent, cnr_Strength.sync.done);
    return ret;
}


XCamReturn sample_acnr_getStrength_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v31_t cnr_Strength;
    cnr_Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetStrength(ctx, &cnr_Strength);
    printf("get acnr v31 attri ret:%d strength:%f done:%d \n\n",
           ret, cnr_Strength.percent, cnr_Strength.sync.done);
    return ret;
}

XCamReturn sample_acnr_setAuto_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v2_t cnrV2_attr;
    cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &cnrV2_attr);
    cnrV2_attr.sync.sync_mode = sync_mode;
    cnrV2_attr.eMode = ACNRV2_OP_MODE_AUTO;
    cnrV2_attr.stAuto.stParams.enable = 1;
    for(int i = 0; i < RK_CNR_V2_MAX_ISO_NUM; i++) {
        cnrV2_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);

        cnrV2_attr.stAuto.stParams.hf_bypass[i] = 0;
        cnrV2_attr.stAuto.stParams.lf_bypass[i] = 0;
        cnrV2_attr.stAuto.stParams.global_gain[i] = 1.0;
        cnrV2_attr.stAuto.stParams.global_gain_alpha[i] = 0.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][0] = 256.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][1] = 256.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][2] = 128.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][3] = 85.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][4] = 64.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][5] = 43.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][6] = 32.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][7] = 21.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][8] = 16.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][9] = 8.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][10] = 4.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][11] = 1.0;
        cnrV2_attr.stAuto.stParams.gain_adj_strength_ratio[i][12] = 1.0;

        cnrV2_attr.stAuto.stParams.color_sat_adj[i] = 40.0;
        cnrV2_attr.stAuto.stParams.color_sat_adj_alpha[i] = 0.8;
        cnrV2_attr.stAuto.stParams.hf_spikes_reducion_strength[i] = 0.5;
        cnrV2_attr.stAuto.stParams.hf_denoise_strength[i] = 16.0;
        cnrV2_attr.stAuto.stParams.hf_color_sat[i] = 1.5;
        cnrV2_attr.stAuto.stParams.hf_denoise_alpha[i] = 0.0;
        cnrV2_attr.stAuto.stParams.hf_bf_wgt_clip[i] = 0;
        cnrV2_attr.stAuto.stParams.thumb_spikes_reducion_strength[i] = 0.5;
        cnrV2_attr.stAuto.stParams.thumb_denoise_strength[i] = 16.0;
        cnrV2_attr.stAuto.stParams.thumb_color_sat[i] = 4.0;
        cnrV2_attr.stAuto.stParams.lf_denoise_strength[i] = 16.0;
        cnrV2_attr.stAuto.stParams.lf_color_sat[i] = 4.0;
        cnrV2_attr.stAuto.stParams.lf_denoise_alpha[i] = 0.5;

        cnrV2_attr.stAuto.stParams.kernel_5x5[0] = 1.0;
        cnrV2_attr.stAuto.stParams.kernel_5x5[1] = 0.8825;
        cnrV2_attr.stAuto.stParams.kernel_5x5[2] = 0.7788;
        cnrV2_attr.stAuto.stParams.kernel_5x5[3] = 0.6065;
        cnrV2_attr.stAuto.stParams.kernel_5x5[4] = 0.3679;

    }

    ret = rk_aiq_user_api2_acnrV2_SetAttrib(ctx, &cnrV2_attr);
    printf("set acnr attri auto ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v2_t get_cnrV2_attr;
    get_cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &get_cnrV2_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnrV2_attr.sync.done);
    return ret;
}



XCamReturn sample_acnr_setAuto_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v30_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV30_OP_MODE_AUTO;

    for(int i = 0; i < RK_CNR_V30_MAX_ISO_NUM; i++) {
        cnr_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);
        cnr_attr.stAuto.stParams.CnrParamsISO[i].enable = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].down_scale_x = 4;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].down_scale_y = 4;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_sigma = 0.01;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_bf_ratio = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].chroma_filter_strength = 0.01;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].chroma_filter_wgt_clip = 0.9;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].anti_chroma_ghost = 0.0313;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].chroma_filter_uv_gain = 0.333;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].wgt_slope = 0.7;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_ratio = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].bf_sigmaR = 0.03;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].bf_uvgain = 3;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].bf_ratio = 0.0625;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].hbf_wgt_clip = 0.0078;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].bf_wgt0_sel = 0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].global_alpha = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].saturation_adj_offset = 0.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].saturation_adj_ratio = 0.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].global_gain = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].global_gain_alpha = 0.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].local_gain_scale = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].global_gain_thumb = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].global_gain_alpha_thumb = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[0] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[1] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[2] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[3] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[4] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[5] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[6] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[7] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[8] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[9] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[10] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[11] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gain_adj_strength_ratio[12] = 1.0;


        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_filter_wgt_coeff[0] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_filter_wgt_coeff[1] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_filter_wgt_coeff[2] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].thumb_filter_wgt_coeff[3] = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[0] = 48;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[1] = 28;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[2] = 6;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[3] = 28;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[4] = 17;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].gaus_coeff[5] = 4;

    }

    ret = rk_aiq_user_api2_acnrV30_SetAttrib(ctx, &cnr_attr);
    printf("set acnr attri auto ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v30_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}




XCamReturn sample_acnr_setAuto_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v31_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV31_OP_MODE_AUTO;

    for(int i = 0; i < RK_CNR_V31_MAX_ISO_NUM; i++) {
        cnr_attr.stAuto.stParams.iso[i] = 50 * pow(2, i);
        cnr_attr.stAuto.stParams.CnrParamsISO[i].enable = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_exgain_bypass = 0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_ds_scaleX = 4;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_ds_scaleY = 4;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_thumbBf_coeff[0] = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_thumbBf_coeff[1] = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_thumbBf_coeff[2] = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_thumbBf_coeff[3] = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loBfFlt_vsigma = 0.01;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loBfFlt_alpha = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_loFlt_coeff[0] = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_loFlt_coeff[1] = 1;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_loFlt_coeff[2] = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFlt_vsigma = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltWgt_maxLimit = 2;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltWgt_minThred = 0.0313;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltUV_gain = 0.333;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltWgt_slope = 0.7;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_alpha = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[0] = 0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[1] = 64;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[2] = 128;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[3] = 256;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[4] = 384;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[5] = 640;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[6] = 896;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltVsigma_idx[7] = 1024;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[0] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[1] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[2] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[3] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[4] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[5] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[6] = 0.02;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_vsigma[7] = 0.02;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltUV_gain = 6;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltCur_wgt = 0.01;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFltWgt_minLimit = 0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_hiFltWgt0_mode = 0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_hiFlt_alpha = 1;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_satAdj_offset = 0.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_satAdj_scale = 0.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_global_gain = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_globalGain_alpha = 0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_localGain_scale = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltGlobalSgm_ratio = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_loFltGlobalSgmRto_alpha = 0.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[0] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[1] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[2] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[3] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[4] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[5] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[6] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[7] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[8] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[9] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[10] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[11] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltSgm_ratio[12] = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[0] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[1] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[2] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[3] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[4] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[5] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[6] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[7] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[8] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[9] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[10] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[11] = 1.0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gainAdjHiFltCur_wgt[12] = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].hw_cnrT_gausFltSigma_en = 0;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_sigma = 1.0;

        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[0] = 0.1758;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[1] = 0.1094;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[2] = 0.0234;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[3] = 0.1094;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[4] = 0.0664;
        cnr_attr.stAuto.stParams.CnrParamsISO[i].sw_cnrT_gausFlt_coeff[5] = 0.0156;

    }

    ret = rk_aiq_user_api2_acnrV31_SetAttrib(ctx, &cnr_attr);
    printf("set acnr attri auto ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v31_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v31 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_setManual_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v2_t cnrV2_attr;
    cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &cnrV2_attr);
    cnrV2_attr.sync.sync_mode = sync_mode;
    cnrV2_attr.eMode = ACNRV2_OP_MODE_MANUAL;
    cnrV2_attr.stManual.stSelect.enable = 1;

    cnrV2_attr.stManual.stSelect.hf_bypass = 0;
    cnrV2_attr.stManual.stSelect.lf_bypass = 0;
    cnrV2_attr.stManual.stSelect.global_gain = 1.0;
    cnrV2_attr.stManual.stSelect.global_gain_alpha = 0.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[0] = 256.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[1] = 256.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[2] = 128.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[3] = 85.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[4] = 64.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[5] = 43.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[6] = 32.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[7] = 21.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[8] = 16.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[9] = 8.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[10] = 4.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[11] = 1.0;
    cnrV2_attr.stManual.stSelect.gain_adj_strength_ratio[12] = 1.0;

    cnrV2_attr.stManual.stSelect.color_sat_adj = 40.0;
    cnrV2_attr.stManual.stSelect.color_sat_adj_alpha = 0.8;
    cnrV2_attr.stManual.stSelect.hf_spikes_reducion_strength = 0.5;
    cnrV2_attr.stManual.stSelect.hf_denoise_strength = 16.0;
    cnrV2_attr.stManual.stSelect.hf_color_sat = 1.5;
    cnrV2_attr.stManual.stSelect.hf_denoise_alpha = 0.0;
    cnrV2_attr.stManual.stSelect.hf_bf_wgt_clip = 0;
    cnrV2_attr.stManual.stSelect.thumb_spikes_reducion_strength = 0.5;
    cnrV2_attr.stManual.stSelect.thumb_denoise_strength = 16.0;
    cnrV2_attr.stManual.stSelect.thumb_color_sat = 4.0;
    cnrV2_attr.stManual.stSelect.lf_denoise_strength = 16.0;
    cnrV2_attr.stManual.stSelect.lf_color_sat = 4.0;
    cnrV2_attr.stManual.stSelect.lf_denoise_alpha = 0.5;

    cnrV2_attr.stManual.stSelect.kernel_5x5[0] = 1.0;
    cnrV2_attr.stManual.stSelect.kernel_5x5[1] = 0.8825;
    cnrV2_attr.stManual.stSelect.kernel_5x5[2] = 0.7788;
    cnrV2_attr.stManual.stSelect.kernel_5x5[3] = 0.6065;
    cnrV2_attr.stManual.stSelect.kernel_5x5[4] = 0.3679;

    ret = rk_aiq_user_api2_acnrV2_SetAttrib(ctx, &cnrV2_attr);
    printf("set cnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v2_t get_cnrV2_attr;
    get_cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &get_cnrV2_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnrV2_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_setManual_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v30_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV30_OP_MODE_MANUAL;
    cnr_attr.stManual.stSelect.enable = 1;
    cnr_attr.stManual.stSelect.down_scale_x = 4;
    cnr_attr.stManual.stSelect.down_scale_y = 4;

    cnr_attr.stManual.stSelect.thumb_sigma = 0.01;
    cnr_attr.stManual.stSelect.thumb_bf_ratio = 1.0;

    cnr_attr.stManual.stSelect.chroma_filter_strength = 0.01;
    cnr_attr.stManual.stSelect.chroma_filter_wgt_clip = 0.9;
    cnr_attr.stManual.stSelect.anti_chroma_ghost = 0.0313;
    cnr_attr.stManual.stSelect.chroma_filter_uv_gain = 0.333;
    cnr_attr.stManual.stSelect.wgt_slope = 0.7;

    cnr_attr.stManual.stSelect.gaus_ratio = 1;
    cnr_attr.stManual.stSelect.bf_sigmaR = 0.03;
    cnr_attr.stManual.stSelect.bf_uvgain = 3;
    cnr_attr.stManual.stSelect.bf_ratio = 0.0625;
    cnr_attr.stManual.stSelect.hbf_wgt_clip = 0.0078;
    cnr_attr.stManual.stSelect.bf_wgt0_sel = 0;
    cnr_attr.stManual.stSelect.global_alpha = 1.0;

    cnr_attr.stManual.stSelect.saturation_adj_offset = 0.0;
    cnr_attr.stManual.stSelect.saturation_adj_ratio = 0.0;

    cnr_attr.stManual.stSelect.global_gain = 1.0;
    cnr_attr.stManual.stSelect.global_gain_alpha = 0.0;
    cnr_attr.stManual.stSelect.local_gain_scale = 1.0;
    cnr_attr.stManual.stSelect.global_gain_thumb = 1.0;
    cnr_attr.stManual.stSelect.global_gain_alpha_thumb = 1.0;


    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[0] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[1] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[2] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[3] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[4] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[5] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[6] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[7] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[8] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[9] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[10] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[11] = 1.0;
    cnr_attr.stManual.stSelect.gain_adj_strength_ratio[12] = 1.0;

    cnr_attr.stManual.stSelect.gaus_coeff[0] = 1;
    cnr_attr.stManual.stSelect.gaus_coeff[1] = 1;
    cnr_attr.stManual.stSelect.gaus_coeff[2] = 1;
    cnr_attr.stManual.stSelect.gaus_coeff[3] = 1;

    cnr_attr.stManual.stSelect.gaus_coeff[0] = 48;
    cnr_attr.stManual.stSelect.gaus_coeff[1] = 28;
    cnr_attr.stManual.stSelect.gaus_coeff[2] = 6;
    cnr_attr.stManual.stSelect.gaus_coeff[3] = 28;
    cnr_attr.stManual.stSelect.gaus_coeff[4] = 17;
    cnr_attr.stManual.stSelect.gaus_coeff[5] = 4;

    ret = rk_aiq_user_api2_acnrV30_SetAttrib(ctx, &cnr_attr);
    printf("set cnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v30_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}


XCamReturn sample_acnr_setManual_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v31_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV31_OP_MODE_MANUAL;
    cnr_attr.stManual.stSelect.enable = 1;
    cnr_attr.stManual.stSelect.hw_cnrT_exgain_bypass = 0;
    cnr_attr.stManual.stSelect.sw_cnrT_ds_scaleX = 4;
    cnr_attr.stManual.stSelect.sw_cnrT_ds_scaleY = 4;

    cnr_attr.stManual.stSelect.sw_cnrT_thumbBf_coeff[0] = 1;
    cnr_attr.stManual.stSelect.sw_cnrT_thumbBf_coeff[1] = 1;
    cnr_attr.stManual.stSelect.sw_cnrT_thumbBf_coeff[2] = 1;
    cnr_attr.stManual.stSelect.sw_cnrT_thumbBf_coeff[3] = 1;

    cnr_attr.stManual.stSelect.sw_cnrT_loBfFlt_vsigma = 0.01;
    cnr_attr.stManual.stSelect.sw_cnrT_loBfFlt_alpha = 1;

    cnr_attr.stManual.stSelect.hw_cnrT_loFlt_coeff[0] = 1;
    cnr_attr.stManual.stSelect.hw_cnrT_loFlt_coeff[1] = 1;
    cnr_attr.stManual.stSelect.hw_cnrT_loFlt_coeff[2] = 1;

    cnr_attr.stManual.stSelect.sw_cnrT_loFlt_vsigma = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_loFltWgt_maxLimit = 2;
    cnr_attr.stManual.stSelect.sw_cnrT_loFltWgt_minThred = 0.0313;

    cnr_attr.stManual.stSelect.sw_cnrT_loFltUV_gain = 0.333;
    cnr_attr.stManual.stSelect.sw_cnrT_loFltWgt_slope = 0.7;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_alpha = 1;

    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[0] = 0;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[1] = 64;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[2] = 128;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[3] = 256;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[4] = 384;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[5] = 640;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[6] = 896;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltVsigma_idx[7] = 1024;

    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[0] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[1] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[2] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[3] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[4] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[5] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[6] = 0.02;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_vsigma[7] = 0.02;

    cnr_attr.stManual.stSelect.sw_cnrT_hiFltUV_gain = 6;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltCur_wgt = 0.01;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFltWgt_minLimit = 0;

    cnr_attr.stManual.stSelect.hw_cnrT_hiFltWgt0_mode = 0;
    cnr_attr.stManual.stSelect.sw_cnrT_hiFlt_alpha = 1;

    cnr_attr.stManual.stSelect.hw_cnrT_satAdj_offset = 0.0;
    cnr_attr.stManual.stSelect.sw_cnrT_satAdj_scale = 0.0;
    cnr_attr.stManual.stSelect.sw_cnrT_global_gain = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_globalGain_alpha = 0;
    cnr_attr.stManual.stSelect.sw_cnrT_localGain_scale = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_loFltGlobalSgm_ratio = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_loFltGlobalSgmRto_alpha = 0.0;

    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[0] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[1] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[2] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[3] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[4] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[5] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[6] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[7] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[8] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[9] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[10] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[11] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltSgm_ratio[12] = 1.0;

    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[0] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[1] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[2] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[3] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[4] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[5] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[6] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[7] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[8] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[9] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[10] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[11] = 1.0;
    cnr_attr.stManual.stSelect.sw_cnrT_gainAdjHiFltCur_wgt[12] = 1.0;

    cnr_attr.stManual.stSelect.hw_cnrT_gausFltSigma_en = 0;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_sigma = 1.0;

    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[0] = 0.1758;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[1] = 0.1094;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[2] = 0.0234;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[3] = 0.1094;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[4] = 0.0664;
    cnr_attr.stManual.stSelect.sw_cnrT_gausFlt_coeff[5] = 0.0156;

    ret = rk_aiq_user_api2_acnrV31_SetAttrib(ctx, &cnr_attr);
    printf("set cnr v31 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v31_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v31 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}


XCamReturn sample_acnr_setReg_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v2_t cnrV2_attr;
    cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &cnrV2_attr);
    cnrV2_attr.sync.sync_mode = sync_mode;
    cnrV2_attr.eMode = ACNRV2_OP_MODE_REG_MANUAL;

    //ISP_CNR_2800_CTR
    cnrV2_attr.stManual.stFix.cnr_thumb_mix_cur_en = 0;
    cnrV2_attr.stManual.stFix.cnr_lq_bila_bypass = 0;
    cnrV2_attr.stManual.stFix.cnr_hq_bila_bypass = 0;
    cnrV2_attr.stManual.stFix.cnr_exgain_bypass = 0;
    cnrV2_attr.stManual.stFix.cnr_en_i = 1;

    // ISP_CNR_2800_EXGAIN
    cnrV2_attr.stManual.stFix.cnr_global_gain_alpha = 0x0;
    cnrV2_attr.stManual.stFix.cnr_global_gain = 0x80;

    // ISP_CNR_2800_GAIN_PARA
    cnrV2_attr.stManual.stFix.cnr_gain_iso = 0x16;
    cnrV2_attr.stManual.stFix.cnr_gain_offset = 0x0c;
    cnrV2_attr.stManual.stFix.cnr_gain_1sigma = 0x3c;

    // ISP_CNR_2800_GAIN_UV_PARA
    cnrV2_attr.stManual.stFix.cnr_gain_uvgain1 = 0x40;
    cnrV2_attr.stManual.stFix.cnr_gain_uvgain0 = 0x30;

    // ISP_CNR_2800_LMED3
    cnrV2_attr.stManual.stFix.cnr_lmed3_alpha = 0x03;

    // ISP_CNR_2800_LBF5_GAIN
    cnrV2_attr.stManual.stFix.cnr_lbf5_gain_y = 0x1;
    cnrV2_attr.stManual.stFix.cnr_lbf5_gain_c = 0x02;

    // ISP_CNR_2800_LBF5_WEITD0_4
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_d[0] = 0x80;
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_d[1] = 0x70;
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_d[2] = 0x63;
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_d[3] = 0x4d;
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_d[4] = 0x2f;

    // ISP_CNR_2800_HMED3
    cnrV2_attr.stManual.stFix.cnr_hmed3_alpha = 0x03;

    // ISP_CNR_2800_HBF5
    cnrV2_attr.stManual.stFix.cnr_hbf5_weit_src = 0x0f;
    cnrV2_attr.stManual.stFix.cnr_hbf5_min_wgt = 0x00;
    cnrV2_attr.stManual.stFix.cnr_hbf5_sigma = 0x0438;

    // ISP_CNR_2800_LBF3
    cnrV2_attr.stManual.stFix.cnr_lbf5_weit_src = 0xff;
    cnrV2_attr.stManual.stFix.cnr_lbf3_sigma = 0x0438;

    //ISP_CNR_2800_SIGMA0-SIGMA3
    cnrV2_attr.stManual.stFix.cnr_sigma_y[0] = 0xff;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[1] = 0x80;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[2] = 0x55;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[3] = 0x40;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[4] = 0x2b;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[5] = 0x20;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[6] = 0x15;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[7] = 0x10;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[8] = 0x08;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[9] = 0x04;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[10] = 0x02;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[11] = 0x01;
    cnrV2_attr.stManual.stFix.cnr_sigma_y[12] = 0x01;

    ret = rk_aiq_user_api2_acnrV2_SetAttrib(ctx, &cnrV2_attr);
    printf("set cnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v2_t get_cnrV2_attr;
    get_cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &get_cnrV2_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnrV2_attr.sync.done);
    return ret;
}



XCamReturn sample_acnr_setReg_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v30_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV30_OP_MODE_REG_MANUAL;


    //CNR_CTRL
    cnr_attr.stManual.stFix.bf3x3_wgt0_sel = 0;
    cnr_attr.stManual.stFix.thumb_mode = 0;
    cnr_attr.stManual.stFix.yuv422_mode = 0;
    cnr_attr.stManual.stFix.exgain_bypass = 0;
    cnr_attr.stManual.stFix.cnr_en = 1;

    // CNR_EXGAIN
    cnr_attr.stManual.stFix.global_gain_alpha = 0x0;
    cnr_attr.stManual.stFix.gain_iso = 0x33;
    cnr_attr.stManual.stFix.global_gain = 0x10;

    // CNR_THUMB1
    cnr_attr.stManual.stFix.thumb_sigma_c = 0x88;
    cnr_attr.stManual.stFix.thumb_sigma_y = 0xa3;

    // CNR_THUMB_BF_RATIO
    cnr_attr.stManual.stFix.thumb_bf_ratio = 0x400;

    // CNR_LBF_WEITD
    cnr_attr.stManual.stFix.lbf1x7_weit_d[0] = 0x80;
    cnr_attr.stManual.stFix.lbf1x7_weit_d[1] = 0x80;
    cnr_attr.stManual.stFix.lbf1x7_weit_d[2] = 0x80;
    cnr_attr.stManual.stFix.lbf1x7_weit_d[3] = 0x80;

    // CNR_IIR_PARA1
    cnr_attr.stManual.stFix.iir_uvgain = 0x05;
    cnr_attr.stManual.stFix.iir_strength = 0xd9;
    cnr_attr.stManual.stFix.exp_shift = 0x06;
    cnr_attr.stManual.stFix.wgt_slope = 0x5a;

    // CNR_IIR_PARA2
    cnr_attr.stManual.stFix.chroma_ghost = 0x02;
    cnr_attr.stManual.stFix.iir_uv_clip = 0x3a;

    // CNR_GAUS_COE
    cnr_attr.stManual.stFix.gaus_coe[0] = 0x30;
    cnr_attr.stManual.stFix.gaus_coe[1] = 0x1c;
    cnr_attr.stManual.stFix.gaus_coe[2] = 0x6;
    cnr_attr.stManual.stFix.gaus_coe[3] = 0x1c;
    cnr_attr.stManual.stFix.gaus_coe[4] = 0x11;
    cnr_attr.stManual.stFix.gaus_coe[5] = 0x4;


    // CNR_GAUS_RATIO
    cnr_attr.stManual.stFix.gaus_ratio = 0x400;
    cnr_attr.stManual.stFix.bf_wgt_clip = 0x01;
    cnr_attr.stManual.stFix.global_alpha = 0x400;

    // CNR_BF_PARA1
    cnr_attr.stManual.stFix.uv_gain = 0x30;
    cnr_attr.stManual.stFix.sigma_r = 0x6c;
    cnr_attr.stManual.stFix.bf_ratio = 0x8;


    // CNR_SIGMA
    cnr_attr.stManual.stFix.adj_offset = 0x00;
    cnr_attr.stManual.stFix.adj_ratio = 0x00;

    //CNR_SIGMA-SIGMA3
    cnr_attr.stManual.stFix.sigma_y[0] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[1] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[2] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[3] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[4] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[5] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[6] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[7] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[8] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[9] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[10] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[11] = 0xff;
    cnr_attr.stManual.stFix.sigma_y[12] = 0xff;

    // CNR_IIR_GLOBAL_GAIN
    cnr_attr.stManual.stFix.iir_gain_alpha = 0x08;
    cnr_attr.stManual.stFix.iir_global_gain = 0x40;

    ret = rk_aiq_user_api2_acnrV30_SetAttrib(ctx, &cnr_attr);
    printf("set cnr v2 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v30_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_setReg_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_attrib_v31_t cnr_attr;
    cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &cnr_attr);
    cnr_attr.sync.sync_mode = sync_mode;
    cnr_attr.eMode = ACNRV31_OP_MODE_REG_MANUAL;

    //CNR_CTRL
    cnr_attr.stManual.stFix.loflt_coeff = 0x3f;
    cnr_attr.stManual.stFix.hiflt_wgt0_mode = 0;
    cnr_attr.stManual.stFix.thumb_mode = 0;
    cnr_attr.stManual.stFix.yuv422_mode = 0;
    cnr_attr.stManual.stFix.exgain_bypass = 0;
    cnr_attr.stManual.stFix.cnr_en = 1;

    // CNR_EXGAIN
    cnr_attr.stManual.stFix.global_gain_alpha = 0x0;
    cnr_attr.stManual.stFix.local_gain_scale = 0x80;
    cnr_attr.stManual.stFix.global_gain = 0x10;

    // CNR_THUMB1
    cnr_attr.stManual.stFix.lobfflt_vsigma_uv = 0x88;
    cnr_attr.stManual.stFix.lobfflt_vsigma_y = 0xa3;

    // CNR_THUMB_BF_RATIO
    cnr_attr.stManual.stFix.lobfflt_alpha = 0x400;

    // CNR_LBF_WEITD
    cnr_attr.stManual.stFix.thumb_bf_coeff[0] = 0x80;
    cnr_attr.stManual.stFix.thumb_bf_coeff[1] = 0x80;
    cnr_attr.stManual.stFix.thumb_bf_coeff[2] = 0x80;
    cnr_attr.stManual.stFix.thumb_bf_coeff[3] = 0x80;

    // CNR_IIR_PARA1
    cnr_attr.stManual.stFix.loflt_uv_gain = 0x05;
    cnr_attr.stManual.stFix.loflt_vsigma = 0xd9;
    cnr_attr.stManual.stFix.exp_x_shift_bit = 0x06;
    cnr_attr.stManual.stFix.loflt_wgt_slope = 0x5a;

    // CNR_IIR_PARA2
    cnr_attr.stManual.stFix.loflt_wgt_max_limit = 0x02;
    cnr_attr.stManual.stFix.loflt_wgt_min_thred = 0x3a;

    // CNR_GAUS_COE
    cnr_attr.stManual.stFix.gaus_flt_coeff[0] = 0x30;
    cnr_attr.stManual.stFix.gaus_flt_coeff[1] = 0x1c;
    cnr_attr.stManual.stFix.gaus_flt_coeff[2] = 0x6;
    cnr_attr.stManual.stFix.gaus_flt_coeff[3] = 0x1c;
    cnr_attr.stManual.stFix.gaus_flt_coeff[4] = 0x11;
    cnr_attr.stManual.stFix.gaus_flt_coeff[5] = 0x4;


    // CNR_GAUS_RATIO
    cnr_attr.stManual.stFix.gaus_flt_alpha = 0x400;
    cnr_attr.stManual.stFix.hiflt_wgt_min_limit = 0x01;
    cnr_attr.stManual.stFix.hiflt_alpha = 0x400;

    // CNR_BF_PARA1
    cnr_attr.stManual.stFix.hiflt_uv_gain = 0x30;
    cnr_attr.stManual.stFix.hiflt_global_vsigma = 0x6c;
    cnr_attr.stManual.stFix.hiflt_cur_wgt = 0x8;


    // CNR_SIGMA
    cnr_attr.stManual.stFix.adj_offset = 0x00;
    cnr_attr.stManual.stFix.adj_scale = 0x00;

    //CNR_SIGMA-SIGMA3
    cnr_attr.stManual.stFix.sgm_ratio[0] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[1] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[2] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[3] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[4] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[5] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[6] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[7] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[8] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[9] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[10] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[11] = 0x10;
    cnr_attr.stManual.stFix.sgm_ratio[12] = 0x10;

    // CNR_IIR_GLOBAL_GAIN
    cnr_attr.stManual.stFix.loflt_global_sgm_ratio_alpha = 0x08;
    cnr_attr.stManual.stFix.loflt_global_sgm_ratio = 0x40;

    // CNR_WGT_SIGMA
    cnr_attr.stManual.stFix.cur_wgt[0] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[1] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[2] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[3] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[4] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[5] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[6] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[7] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[8] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[9] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[10] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[11] = 0x80;
    cnr_attr.stManual.stFix.cur_wgt[12] = 0x80;

    cnr_attr.stManual.stFix.hiflt_vsigma_idx[0] = 0x00;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[1] = 0x10;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[2] = 0x80;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[3] = 0x100;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[4] = 0x180;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[5] = 0x280;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[6] = 0x380;
    cnr_attr.stManual.stFix.hiflt_vsigma_idx[7] = 0x3ff;


    cnr_attr.stManual.stFix.hiflt_vsigma[0] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[1] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[2] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[3] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[4] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[5] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[6] = 0x2a8;
    cnr_attr.stManual.stFix.hiflt_vsigma[7] = 0x2a8;

    ret = rk_aiq_user_api2_acnrV31_SetAttrib(ctx, &cnr_attr);
    printf("set cnr v31 attri manual ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v31_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v31 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);
    return ret;
}

XCamReturn sample_acnr_setStrength_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v2_t cnrV2_Strenght;
    cnrV2_Strenght.sync.sync_mode = sync_mode;
    cnrV2_Strenght.percent = fStrength;
    ret = rk_aiq_user_api2_acnrV2_SetStrength(ctx, &cnrV2_Strenght);
    printf("Set acnr v2 set streangth ret:%d strength:%f \n\n", ret, cnrV2_Strenght.percent);

    rk_aiq_cnr_strength_v2_t get_cnrV2_Strenght;
    get_cnrV2_Strenght.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetStrength(ctx, &get_cnrV2_Strenght);
    printf("get acnr v2 attri ret:%d strength:%f done:%d \n\n",
           ret, get_cnrV2_Strenght.percent, get_cnrV2_Strenght.sync.done);
    return ret;
}

XCamReturn sample_acnr_setStrength_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v30_t cnr_Strength;
    cnr_Strength.sync.sync_mode = sync_mode;
    cnr_Strength.percent = fStrength;
    cnr_Strength.strength_enable = true;
    ret = rk_aiq_user_api2_acnrV30_SetStrength(ctx, &cnr_Strength);
    printf("Set acnr v30 set streangth ret:%d strength:%f \n\n", ret, cnr_Strength.percent);

    rk_aiq_cnr_strength_v30_t get_cnr_Strength;
    get_cnr_Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetStrength(ctx, &get_cnr_Strength);
    printf("get acnr v30 attri ret:%d strength:%f done:%d \n\n",
           ret, get_cnr_Strength.percent, get_cnr_Strength.sync.done);
    return ret;
}

XCamReturn sample_acnr_setStrength_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, float fStrength)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_cnr_strength_v31_t cnr_Strength;
    cnr_Strength.sync.sync_mode = sync_mode;
    cnr_Strength.percent = fStrength;
    cnr_Strength.strength_enable = true;
    ret = rk_aiq_user_api2_acnrV31_SetStrength(ctx, &cnr_Strength);
    printf("Set acnr v31 set streangth ret:%d strength:%f \n\n", ret, cnr_Strength.percent);

    rk_aiq_cnr_strength_v31_t get_cnr_Strength;
    get_cnr_Strength.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetStrength(ctx, &get_cnr_Strength);
    printf("get acnr v31 attri ret:%d strength:%f done:%d \n\n",
           ret, get_cnr_Strength.percent, get_cnr_Strength.sync.done);
    return ret;
}

XCamReturn sample_acnr_setDefault_v2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_cnr_attrib_v2_t* default_cnrV2_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    default_cnrV2_attr->sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_SetAttrib(ctx, default_cnrV2_attr);
    printf("Set acnr v2 set default attri ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v2_t get_cnrV2_attr;
    get_cnrV2_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &get_cnrV2_attr);
    printf("get acnr v2 attri ret:%d done:%d\n\n", ret, get_cnrV2_attr.sync.done);

    return ret;
}

XCamReturn sample_acnr_setDefault_v30(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_cnr_attrib_v30_t * default_cnr_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    default_cnr_attr->sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_SetAttrib(ctx, default_cnr_attr);
    printf("Set acnr v30 set default attri ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v30_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v30 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);

    return ret;
}


XCamReturn sample_acnr_setDefault_v31(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode, rk_aiq_cnr_attrib_v31_t * default_cnr_attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    default_cnr_attr->sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_SetAttrib(ctx, default_cnr_attr);
    printf("Set acnr v31 set default attri ret:%d \n\n", ret);

    rk_aiq_cnr_attrib_v31_t get_cnr_attr;
    get_cnr_attr.sync.sync_mode = sync_mode;
    ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &get_cnr_attr);
    printf("get acnr v31 attri ret:%d done:%d\n\n", ret, get_cnr_attr.sync.done);

    return ret;
}

#ifdef USE_NEWSTRUCT

static void get_auto_attr(cnr_api_attrib_t* attr) {
    cnr_param_auto_t* stAuto = &attr->stAuto;
    for (int i = 0;i < 13;i++) {
    }
}

static void get_manual_attr(cnr_api_attrib_t* attr) {
    cnr_param_t* stMan = &attr->stMan;
}

void sample_cnr_reverseEn(const rk_aiq_sys_ctx_t* ctx)
{
    // get cur mode
    printf("+++++++ cnr module test start ++++++++\n");

    cnr_api_attrib_t attr;
    memset(&attr, 0, sizeof(attr));

    rk_aiq_user_api2_cnr_GetAttrib(ctx, &attr);

    printf("cnr attr: opmode:%d, en:%d, bypass:%d\n", attr.opMode, attr.en, attr.bypass);

    srand(time(0));
    int rand_num = rand() % 101;

    if (rand_num <70) {
        printf("update cnr arrrib!\n");
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
        printf("reverse cnr en!\n");
        attr.en = !attr.en;
    }

    rk_aiq_user_api2_cnr_SetAttrib(ctx, &attr);

    // wait more than 2 frames
    usleep(90 * 1000);

    cnr_status_t status;
    memset(&status, 0, sizeof(cnr_status_t));

    rk_aiq_user_api2_cnr_QueryStatus(ctx, &status);

    printf("cnr status: opmode:%d, en:%d, bypass:%d\n", status.opMode, status.en, status.bypass);

    printf("-------- cnr module test done --------\n");
}

void sample_cnr_setStrength_new(const rk_aiq_sys_ctx_t* ctx, float percent) {
    cnr_api_attrib_t attr;
    memset(&attr, 0, sizeof(attr));

    rk_aiq_user_api2_cnr_GetAttrib(ctx, &attr);

    printf("cnr attr: opmode:%d, en:%d, bypass:%d\n", attr.opMode, attr.en, attr.bypass);
    if (attr.opMode == RK_AIQ_OP_MODE_MANUAL || attr.en == false) {
        attr.opMode = RK_AIQ_OP_MODE_AUTO;
        attr.en = true;
        rk_aiq_user_api2_cnr_SetAttrib(ctx, &attr);
        usleep(90 * 1000);
    }

    acnr_strength_t cnrStrength;
    cnrStrength.en = true;
    cnrStrength.percent = percent;
    rk_aiq_user_api2_cnr_SetStrength(ctx, &cnrStrength);

    printf("set percent %f\n", cnrStrength.percent);
}
#endif

XCamReturn sample_acnr_module (const void *arg)
{
    int key = -1;
    CLEAR();
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx ;

    if(demo_ctx->camGroup) {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
        printf("##################group !!!!########################\n");
    } else {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
        printf("##################sigle !!!!########################\n");
    }

    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    rk_aiq_cnr_attrib_v2_t default_cnrV2_attr;
    if (CHECK_ISP_HW_V30()) {
        ret = rk_aiq_user_api2_acnrV2_GetAttrib(ctx, &default_cnrV2_attr);
        printf("get acnr v2 default attri ret:%d \n\n", ret);
    }

    rk_aiq_cnr_attrib_v30_t default_cnrV30_attr;
    if ( CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
        ret = rk_aiq_user_api2_acnrV30_GetAttrib(ctx, &default_cnrV30_attr);
        printf("get acnr v30 default attri ret:%d \n\n", ret);
    }

    rk_aiq_cnr_attrib_v31_t default_cnrV31_attr;
    if ( CHECK_ISP_HW_V39()) {
        ret = rk_aiq_user_api2_acnrV31_GetAttrib(ctx, &default_cnrV31_attr);
        printf("get acnr v31 default attri ret:%d \n\n", ret);
    }
    do {
        sample_acnr_usage ();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar ();
        printf ("\n");

        switch (key) {
        case '0':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_getAtrr_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_getAtrr_v30(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_getAtrr_v31(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '1':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_getStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_getStrength_v30(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_getStrength_v31(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '2':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setAuto_v30(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setAuto_v31(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '3':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setManual_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setManual_v30(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setManual_v31(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '4':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setReg_v2(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setReg_v30(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setReg_v31(ctx, RK_AIQ_UAPI_MODE_SYNC);
            }
            break;
        case '5':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_SYNC, 1.0);
            }
#if USE_NEWSTRUCT
            sample_cnr_setStrength_new(ctx, 1.0);
#endif
            break;
        case '6':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.0);
            }
#if USE_NEWSTRUCT
            sample_cnr_setStrength_new(ctx, 0.0);
#endif
            break;
        case '7':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_SYNC, 0.5);
            }
#if USE_NEWSTRUCT
            sample_cnr_setStrength_new(ctx, 0.5);
#endif
            break;
        case '8':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_SYNC, &default_cnrV2_attr);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setDefault_v30(ctx, RK_AIQ_UAPI_MODE_SYNC, &default_cnrV30_attr);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setDefault_v31(ctx, RK_AIQ_UAPI_MODE_SYNC, &default_cnrV31_attr);
            }
            break;
        case 'a':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_getAtrr_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_getAtrr_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_getAtrr_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'b':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_getStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_getStrength_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_getStrength_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'c':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setAuto_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setAuto_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setAuto_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'd':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setManual_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setManual_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setManual_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'e':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setReg_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setReg_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setReg_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC);
            }
            break;
        case 'f':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC, 1.0);
            }
            break;
        case 'g':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.0);
            }
            break;
        case 'h':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setStrength_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setStrength_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setStrength_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC, 0.5);
            }
            break;
        case 'i':
            if (CHECK_ISP_HW_V30()) {
                sample_acnr_setDefault_v2(ctx, RK_AIQ_UAPI_MODE_ASYNC, &default_cnrV2_attr);
            }
            if (CHECK_ISP_HW_V32() || CHECK_ISP_HW_V32_LITE()) {
                sample_acnr_setDefault_v30(ctx, RK_AIQ_UAPI_MODE_ASYNC, &default_cnrV30_attr);
            }
            if (CHECK_ISP_HW_V39()) {
                sample_acnr_setDefault_v31(ctx, RK_AIQ_UAPI_MODE_ASYNC, &default_cnrV31_attr);
            }
            break;
#ifdef USE_NEWSTRUCT
        case 'j':
            sample_cnr_reverseEn(ctx);
            break;
#endif
        default:
            printf("not support test\n\n");
            break;
        }

    } while (key != 'q' && key != 'Q');

    return ret;
}

