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

#include "RkAiqCalibDbTypes.h"
#include "RkAiqCalibDbTypesV2.h"
#include "RkAiqCalibDbV2Helper.h"
#include "algo_types_priv.h"
#include "c_base/aiq_base.h"
#include "enh_types_prvt.h"
#include "interpolation.h"
#include "xcam_log.h"

XCamReturn EnhSelectParam(EnhContext_t* pEnhCtx, enh_param_t* out, int iso) {
    LOGI_ADEHAZE("%s(%d): enter!\n", __FUNCTION__, __LINE__);

    if (pEnhCtx == NULL) {
        LOGE_ADEHAZE("%s(%d): null pointer\n", __FUNCTION__, __LINE__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    int i       = 0;
    int iso_low = 0, iso_high = 0, ilow = 0, ihigh = 0, inear = 0;
    float ratio = 0.0f;
    uint16_t uratio;
    enh_param_auto_t* paut = &pEnhCtx->enh_attrib->stAuto;

    pre_interp(iso, NULL, 0, &ilow, &ihigh, &ratio);
    uratio = ratio * (1 << RATIO_FIXBIT);

    if (ratio > 0.5)
        inear = ihigh;
    else
        inear = ilow;

    out->dyn.iir.hw_enhT_iir_inv_sigma =
        interpolation_f32(paut->dyn[ilow].iir.hw_enhT_iir_inv_sigma,
                          paut->dyn[ihigh].iir.hw_enhT_iir_inv_sigma, ratio);
    out->dyn.iir.hw_enhT_iir_soft_thed =
        interpolation_f32(paut->dyn[ilow].iir.hw_enhT_iir_soft_thed,
                          paut->dyn[ihigh].iir.hw_enhT_iir_soft_thed, ratio);
    out->dyn.iir.hw_enhT_iir_cur_wgt = interpolation_f32(
        paut->dyn[ilow].iir.hw_enhT_iir_cur_wgt, paut->dyn[ihigh].iir.hw_enhT_iir_cur_wgt, ratio);

    out->dyn.loBlf.hw_enhT_loBlf_bypass =
        interpolation_bool(paut->dyn[ilow].loBlf.hw_enhT_loBlf_bypass,
                           paut->dyn[ihigh].loBlf.hw_enhT_loBlf_bypass, uratio);
    out->dyn.loBlf.hw_enhT_loBlf_inv_sigma =
        interpolation_f32(paut->dyn[ilow].loBlf.hw_enhT_loBlf_inv_sigma,
                          paut->dyn[ihigh].loBlf.hw_enhT_loBlf_inv_sigma, ratio);
    out->dyn.loBlf.hw_enhT_loBlf_cur_wgt =
        interpolation_f32(paut->dyn[ilow].loBlf.hw_enhT_loBlf_cur_wgt,
                          paut->dyn[ihigh].loBlf.hw_enhT_loBlf_cur_wgt, ratio);
    out->dyn.loBlf.hw_enhT_loBlf_thumb_cur_wgt =
        interpolation_f32(paut->dyn[ilow].loBlf.hw_enhT_loBlf_thumb_cur_wgt,
                          paut->dyn[ihigh].loBlf.hw_enhT_loBlf_thumb_cur_wgt, ratio);

    out->dyn.midBlf.hw_enhT_midBlf_inv_sigma =
        interpolation_f32(paut->dyn[ilow].midBlf.hw_enhT_midBlf_inv_sigma,
                          paut->dyn[ihigh].midBlf.hw_enhT_midBlf_inv_sigma, ratio);
    out->dyn.midBlf.hw_enhT_midBlf_cur_wgt =
        interpolation_f32(paut->dyn[ilow].midBlf.hw_enhT_midBlf_cur_wgt,
                          paut->dyn[ihigh].midBlf.hw_enhT_midBlf_cur_wgt, ratio);

    out->dyn.strg.hw_enhT_global_strg = interpolation_f32(
        paut->dyn[ilow].strg.hw_enhT_global_strg, paut->dyn[ihigh].strg.hw_enhT_global_strg, ratio);
    out->dyn.strg.hw_enhT_detail2strg_en =
        interpolation_bool(paut->dyn[ilow].strg.hw_enhT_detail2strg_en,
                           paut->dyn[ihigh].strg.hw_enhT_detail2strg_en, uratio);
    for (i = 0; i < 8; i++) {
        out->dyn.strg.hw_enhT_detail2strg_curve.idx[i] =
            interpolation_f32(paut->dyn[ilow].strg.hw_enhT_detail2strg_curve.idx[i],
                              paut->dyn[ihigh].strg.hw_enhT_detail2strg_curve.idx[i], ratio);
        out->dyn.strg.hw_enhT_detail2strg_curve.val[i] =
            interpolation_f32(paut->dyn[ilow].strg.hw_enhT_detail2strg_curve.val[i],
                              paut->dyn[ihigh].strg.hw_enhT_detail2strg_curve.val[i], ratio);
    }
    out->dyn.strg.hw_enhT_luma2strg_en =
        interpolation_bool(paut->dyn[ilow].strg.hw_enhT_luma2strg_en,
                           paut->dyn[ihigh].strg.hw_enhT_luma2strg_en, uratio);
    for (i = 0; i < 17; i++) {
        out->dyn.strg.hw_enhT_lum2strg[i] =
            interpolation_f32(paut->dyn[ilow].strg.hw_enhT_lum2strg[i],
                              paut->dyn[ihigh].strg.hw_enhT_lum2strg[i], ratio);
    }
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn create_context(RkAiqAlgoContext** context, const AlgoCtxInstanceCfg* cfg) {
    XCamReturn result                 = XCAM_RETURN_NO_ERROR;
    CamCalibDbV2Context_t* pCalibDbV2 = cfg->calibv2;

    EnhContext_t* ctx = aiq_mallocz(sizeof(EnhContext_t));
    if (ctx == NULL) {
        LOGE_ADEHAZE("%s: create Enh context fail!\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_MEM;
    }

    ctx->isReCal_   = true;
    ctx->enh_attrib = (enh_api_attrib_t*)(CALIBDBV2_GET_MODULE_PTR(pCalibDbV2, enh));

    *context = (RkAiqAlgoContext*)ctx;
    LOGV_ADEHAZE("%s: (exit)\n", __FUNCTION__);
    return result;
}

static XCamReturn destroy_context(RkAiqAlgoContext* context) {
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    EnhContext_t* pEnhCtx = (EnhContext_t*)context;
    aiq_free(pEnhCtx);
    return result;
}

static XCamReturn prepare(RkAiqAlgoCom* params) {
    XCamReturn result     = XCAM_RETURN_NO_ERROR;
    EnhContext_t* pEnhCtx = (EnhContext_t*)params->ctx;

    pEnhCtx->enh_attrib =
        (enh_api_attrib_t*)(CALIBDBV2_GET_MODULE_PTR(params->u.prepare.calibv2, enh));
    pEnhCtx->isReCal_ = true;

    return result;
}

XCamReturn Aenh_processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams, int iso) {
    XCamReturn result = XCAM_RETURN_NO_ERROR;

    EnhContext_t* pEnhCtx        = (EnhContext_t*)inparams->ctx;
    enh_api_attrib_t* enh_attrib = pEnhCtx->enh_attrib;
    enh_param_t* enhRes          = outparams->algoRes;

    LOGV_ADEHAZE("%s: (enter)\n", __FUNCTION__);

    if (!enh_attrib) {
        LOGE_ADEHAZE("enh_attrib is NULL !");
        return XCAM_RETURN_ERROR_MEM;
    }

    bool init     = inparams->u.proc.init;
    int delta_iso = (int)abs(iso - pEnhCtx->iso);

    outparams->cfg_update = false;

    if (inparams->u.proc.is_bw_sensor) {
        enh_attrib->en        = false;
        outparams->cfg_update = init ? true : false;
        return XCAM_RETURN_NO_ERROR;
    }

    if (enh_attrib->opMode != RK_AIQ_OP_MODE_AUTO) {
        LOGE_ADEHAZE("mode is %d, not auto mode, ignore", enh_attrib->opMode);
        return XCAM_RETURN_NO_ERROR;
    }

    if (inparams->u.proc.is_attrib_update) {
        pEnhCtx->isReCal_ = true;
    }

    if (delta_iso >= 1) {
        pEnhCtx->isReCal_ = true;
    }

    if (pEnhCtx->isReCal_) {
        EnhSelectParam(pEnhCtx, enhRes, iso);

        outparams->cfg_update = true;
        outparams->en         = enh_attrib->en;
        outparams->bypass     = enh_attrib->bypass;
        LOGI_ADEHAZE("enh en:%d, bypass:%d", outparams->en, outparams->bypass);
    }

    pEnhCtx->iso      = iso;
    pEnhCtx->isReCal_ = false;

    LOGV_ADEHAZE("%s: (exit)\n", __FUNCTION__);
    return result;
}

static XCamReturn processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams) {
    int iso = inparams->u.proc.iso;
    Aenh_processing(inparams, outparams, iso);

    return XCAM_RETURN_NO_ERROR;
}
#if 0
XCamReturn
algo_enh_SetAttrib
(
    RkAiqAlgoContext* ctx,
    enh_api_attrib_t *attr
) {
    if(ctx == NULL || attr == NULL) {
        LOGE_ADEHAZE("%s(%d): null pointer\n", __FUNCTION__, __LINE__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    EnhContext_t* pEnhCtx = (EnhContext_t*)ctx;
    pEnhCtx->isReCal_ = true;
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
algo_enh_GetAttrib
(
    RkAiqAlgoContext* ctx,
    enh_api_attrib_t* attr
)
{
    if(ctx == NULL || attr == NULL) {
        LOGE_ADEHAZE("%s(%d): null pointer\n", __FUNCTION__, __LINE__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    EnhContext_t* pEnhCtx = (EnhContext_t*)ctx;
    enh_api_attrib_t* enh_attrib = pEnhCtx->enh_attrib;

    attr->opMode = enh_attrib->opMode;
    attr->en = enh_attrib->en;
    attr->bypass = enh_attrib->bypass;
    memcpy(&attr->stAuto, &enh_attrib->stAuto, sizeof(enh_param_auto_t));

    return XCAM_RETURN_NO_ERROR;
}
#endif

#define RKISP_ALGO_ENH_VERSION     "v0.1.0"
#define RKISP_ALGO_ENH_VENDOR      "Rockchip"
#define RKISP_ALGO_ENH_DESCRIPTION "Rockchip dehaze algo for ISP2.0"

RkAiqAlgoDescription g_RkIspAlgoDescEnh = {
    .common =
        {
            .version         = RKISP_ALGO_ENH_VERSION,
            .vendor          = RKISP_ALGO_ENH_VENDOR,
            .description     = RKISP_ALGO_ENH_DESCRIPTION,
            .type            = RK_AIQ_ALGO_TYPE_AENH,
            .id              = 0,
            .create_context  = create_context,
            .destroy_context = destroy_context,
        },
    .prepare      = prepare,
    .pre_process  = NULL,
    .processing   = processing,
    .post_process = NULL,
};

// RKAIQ_END_DECLARE
