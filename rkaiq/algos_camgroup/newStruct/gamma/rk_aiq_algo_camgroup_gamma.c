/*
 * rk_aiq_algo_camgroup_gamma.c
 *
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

#include "rk_aiq_algo_camgroup_types.h"
#include "newStruct/gamma/rk_aiq_algo_camgroup_gamma.h"
#include "newStruct/gamma/gamma_types_prvt.h"
#include "newStruct/rk_aiq_algo_camgroup_common.h"

typedef GammaContext_t GammaGroupContext_t;

static XCamReturn groupGammaProcessing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    LOGD_AGAMMA("%s enter", __FUNCTION__ );

    GammaGroupContext_t* pGammaGroupCtx = (GammaGroupContext_t *)inparams->ctx;
    RkAiqAlgoCamGroupProcIn* procParaGroup = (RkAiqAlgoCamGroupProcIn*)inparams;
    RkAiqAlgoCamGroupProcOut* procResParaGroup = (RkAiqAlgoCamGroupProcOut*)outparams;

    //group empty
    if(procParaGroup == NULL || procParaGroup->camgroupParmasArray == NULL) {
        LOGE_AGAMMA("procParaGroup or camgroupParmasArray is null");
        return(XCAM_RETURN_ERROR_FAILED);
    }

    // skip group algo if in MANUAL mode
    if (pGammaGroupCtx->gamma_attrib->opMode != RK_AIQ_OP_MODE_AUTO) {
        return XCAM_RETURN_NO_ERROR;
    }

    if (procParaGroup->attribUpdated) {
        LOGI("%s attribUpdated", __func__);
        pGammaGroupCtx->isReCal_ = true;
    }

    gamma_param_t* gamma_param = procResParaGroup->camgroupParmasArray[0]->gamma;

    if (pGammaGroupCtx->isReCal_) {
        GammaSelectParam(&pGammaGroupCtx->gamma_attrib->stAuto, gamma_param);
		outparams->cfg_update = true;
        outparams->en = pGammaGroupCtx->gamma_attrib->en;
        outparams->bypass = pGammaGroupCtx->gamma_attrib->bypass;
	} else {
		outparams->cfg_update = false;
	}

	void* gp_ptrs[procResParaGroup->arraySize];
	int gp_size = sizeof(*procResParaGroup->camgroupParmasArray[0]->gamma);
	for (int i = 0; i < procResParaGroup->arraySize; i++)
		gp_ptrs[i] = procResParaGroup->camgroupParmasArray[i]->gamma;

	algo_camgroup_update_results(inparams, outparams, gp_ptrs, gp_size);

    pGammaGroupCtx->isReCal_ = false;

    LOGD_AGAMMA("%s exit\n", __FUNCTION__);
    return ret;
}

#define RKISP_ALGO_CAMGROUP_GAMMA_VERSION     "v0.0.1"
#define RKISP_ALGO_CAMGROUP_GAMMA_VENDOR      "Rockchip"
#define RKISP_ALGO_CAMGROUP_GAMMA_DESCRIPTION "Rockchip Gamma camgroup algo for ISP2.0"

RkAiqAlgoDescription g_RkIspAlgoDescCamgroupGamma = {
    .common = {
        .version = RKISP_ALGO_CAMGROUP_GAMMA_VERSION,
        .vendor  = RKISP_ALGO_CAMGROUP_GAMMA_VENDOR,
        .description = RKISP_ALGO_CAMGROUP_GAMMA_DESCRIPTION,
        .type    = RK_AIQ_ALGO_TYPE_AGAMMA,
        .id      = 0,
        .create_context  = algo_camgroup_CreateCtx,
        .destroy_context = algo_camgroup_DestroyCtx,
    },
    .prepare = algo_camgroup_Prepare,
    .pre_process = NULL,
    .processing = groupGammaProcessing,
    .post_process = NULL,
};
