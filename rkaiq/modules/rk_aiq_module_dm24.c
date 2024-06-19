#include "rk_aiq_isp39_modules.h"

void rk_aiq_dm24_params_cvt(void* attr, isp_params_t* isp_params, common_cvt_info_t *cvtinfo)
{
    struct isp33_debayer_cfg *phwcfg = &isp_params->isp_cfg->others.debayer_cfg;
    dm_param_t *dm_param = (dm_param_t *) attr;
    dm_params_dyn_t *pdyn = &dm_param->dyn;
    dm_params_static_t *psta = &dm_param->sta;

    /* CONTROL */
    phwcfg->g_out_flt_en = pdyn->hw_dmT_gOutlsFlt_en;

    /* LUMA_DX */
    phwcfg->luma_dx[0] = 8;
    phwcfg->luma_dx[1] = 8;
    phwcfg->luma_dx[2] = 9;
    phwcfg->luma_dx[3] = 9;
    phwcfg->luma_dx[4] = 10;
    phwcfg->luma_dx[5] = 10;
    phwcfg->luma_dx[6] = 9;

    /* G_INTERP */
    phwcfg->g_interp_clip_en = pdyn->gInterp.hw_dmT_gInterpClip_en;
    phwcfg->hi_texture_thred  = pdyn->gDrctAlpha.hw_dmT_hiTexture_thred;
    phwcfg->hi_drct_thred       = pdyn->gDrctAlpha.hw_dmT_hiDrct_thred;
    phwcfg->lo_drct_thred       = pdyn->gDrctAlpha.hw_dmT_loDrct_thred;
    phwcfg->drct_method_thred = pdyn->gDrctAlpha.hw_dmT_drctMethod_thred;
    phwcfg->g_interp_sharp_strg_max_limit   = pdyn->gInterp.hw_dmT_gInterpSharpStrg_maxLim;

    /* G_INTERP_FILTER1 */
    phwcfg->lo_drct_flt_coeff1 = psta->gInterp.hw_dmT_loDrctFlt_coeff[0];
    phwcfg->lo_drct_flt_coeff2 = psta->gInterp.hw_dmT_loDrctFlt_coeff[1];
    phwcfg->lo_drct_flt_coeff3 = psta->gInterp.hw_dmT_loDrctFlt_coeff[2];
    phwcfg->lo_drct_flt_coeff4 = psta->gInterp.hw_dmT_loDrctFlt_coeff[3];

    /* G_INTERP_FILTER2 */
    phwcfg->hi_drct_flt_coeff1 = psta->gInterp.hw_dmT_hiDrctFlt_coeff[0];
    phwcfg->hi_drct_flt_coeff2 = psta->gInterp.hw_dmT_hiDrctFlt_coeff[1];
    phwcfg->hi_drct_flt_coeff3 = psta->gInterp.hw_dmT_hiDrctFlt_coeff[2];
    phwcfg->hi_drct_flt_coeff4 = psta->gInterp.hw_dmT_hiDrctFlt_coeff[3];

    /* G_INTERP_OFFSET_ALPHA */
    phwcfg->g_interp_sharp_strg_offset = pdyn->gInterp.hw_dmT_gInterpSharpStrg_offset;
    phwcfg->grad_lo_flt_alpha = (pdyn->gDrctAlpha.hw_dmT_gradLoFlt_alpha * (1 << RK_DM23_FIX_BIT_GRAD_FLT_ALPHA));

    /* G_INTERP_DRCT_OFFSET */
    phwcfg->drct_offset[0] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[0], 1);
    phwcfg->drct_offset[1] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[1], 1);
    phwcfg->drct_offset[2] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[2], 1);
    phwcfg->drct_offset[3] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[3], 1);
    phwcfg->drct_offset[4] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[4], 1);
    phwcfg->drct_offset[5] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[5], 1);
    phwcfg->drct_offset[6] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[6], 1);
    phwcfg->drct_offset[7] = MAX(pdyn->gDrctAlpha.hw_dmT_luma2Drct_offset[7], 1);

    /* G_FILTER_MODE_OFFSET */
    phwcfg->gflt_mode = pdyn->hw_dmT_gOutlsFlt_mode;
    phwcfg->gflt_ratio = ROUND_F(pdyn->gOutlsFlt_bifilt.hw_dmT_bifilt_alpha * (1 << RK_DM23_FIX_BIT_BF_RATIO));
    phwcfg->gflt_offset = pdyn->gOutlsFlt_maxMin.hw_dmT_gOutlsFltRange_offset;

    /* G_FILTER_FILTER */
    int filter_coe[3];
    if (pdyn->gOutlsFlt_bifilt.sw_dmT_filtCfg_mode == dm_cfgByFiltStrg_mode) {
        float dis_table_3x3[3] = { 0.0, 1.0, 2.0};
        float gaus_table[3];

        float sigma = pdyn->gOutlsFlt_bifilt.sw_dmT_filtSpatial_strg;
        double e = 2.71828182845905;
        for (int k = 0; k < 3; k++)
        {
            float tmp = pow(e, -dis_table_3x3[k] / 2.0 / sigma / sigma);
            gaus_table[k] = tmp;
        }

        float sumTable = 0;
        sumTable = gaus_table[0]
                   + 4 * gaus_table[1]
                   + 4 * gaus_table[2];

        for (int k = 0; k < 3; k++) {
            gaus_table[k] = gaus_table[k] / sumTable;
            filter_coe[k] = ROUND_F(gaus_table[k] * (1 << RK_DM23_FIX_BIT_BF_WGT));
        }
    } else {
        filter_coe[0] = ROUND_F(ROUND_F(pdyn->gOutlsFlt_bifilt.hw_dmT_filtSpatial_wgt[0]) * (1 << RK_DM23_FIX_BIT_BF_WGT));
        filter_coe[1] = ROUND_F(ROUND_F(pdyn->gOutlsFlt_bifilt.hw_dmT_filtSpatial_wgt[1]) * (1 << RK_DM23_FIX_BIT_BF_WGT));
        filter_coe[2] = ROUND_F(ROUND_F(pdyn->gOutlsFlt_bifilt.hw_dmT_filtSpatial_wgt[2]) * (1 << RK_DM23_FIX_BIT_BF_WGT));
    }

    // check filter coeff
    int sum_coeff, offset;
    sum_coeff = filter_coe[0] + 4 * filter_coe[1] + 4 * filter_coe[2];
    offset = (1 << RK_DM23_FIX_BIT_BF_WGT) - sum_coeff;
    filter_coe[0] = filter_coe[0] + offset;

    phwcfg->gflt_coe0 = filter_coe[0];
    phwcfg->gflt_coe1 = filter_coe[1];
    phwcfg->gflt_coe2 = filter_coe[2];

    /* G_FILTER_VSIGMA */
    phwcfg->gflt_vsigma[0] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[0]);
    phwcfg->gflt_vsigma[1] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[1]);
    phwcfg->gflt_vsigma[2] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[2]);
    phwcfg->gflt_vsigma[3] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[3]);
    phwcfg->gflt_vsigma[4] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[4]);
    phwcfg->gflt_vsigma[5] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[5]);
    phwcfg->gflt_vsigma[6] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[6]);
    phwcfg->gflt_vsigma[7] = ROUND_F((1 << RK_DM23_FIX_BIT_INV_BF_SIGMA) / pdyn->gOutlsFlt_bifilt.hw_dmT_luma2RgeSgm_val[7]);
    return;
}
