/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */

#include "newStruct/dehaze/include/dehaze_algo_api.h"
#include "uAPI2/rk_aiq_user_api2_dehaze.h"
#include "aiq_core_c/algo_handlers/RkAiqDehazeHandler.h"
#include "include/uAPI2/rk_aiq_user_api2_imgproc.h"
#include "uAPI2_c/rk_aiq_user_api2_common.h"

RKAIQ_BEGIN_DECLARE

#ifdef RK_SIMULATOR_HW
#define CHECK_USER_API_ENABLE
#endif

/*
*****************************
*
* Desc: set/get manual dehaze strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
static XCamReturn
_dehaze_SetMDehazeStrth(const rk_aiq_sys_ctx_t* sys_ctx, adehaze_strength_t ctrl)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    AiqDehazeHandler_t* algo_handle =
        (AiqDehazeHandler_t*)sys_ctx->_analyzer->mAlgoHandleMaps[RK_AIQ_ALGO_TYPE_ADHAZ];
    if (algo_handle) {
        return AiqDehazeHandler_setStrength(algo_handle, &ctrl);
    }
    return ret;
}

static XCamReturn
_dehaze_GetMDehazeStrth(const rk_aiq_sys_ctx_t* sys_ctx, adehaze_strength_t *ctrl)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    AiqDehazeHandler_t* algo_handle =
        (AiqDehazeHandler_t*)sys_ctx->_analyzer->mAlgoHandleMaps[RK_AIQ_ALGO_TYPE_ADHAZ];
    if (algo_handle) {
        return AiqDehazeHandler_getStrength(algo_handle, ctrl);
    }
    return ret;
}


XCamReturn rk_aiq_uapi2_setDehazeEnhanceStrth(const rk_aiq_sys_ctx_t* sys_ctx, adehaze_strength_t ctrl)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    CHECK_USER_API_ENABLE2(sys_ctx);
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ADHAZ);
    RKAIQ_API_SMART_LOCK(sys_ctx);
	rk_aiq_sys_ctx_array_t ctx_array = rk_aiq_user_api2_common_getSysCtxArray(sys_ctx);
    for (int i = 0; i < ctx_array.num; i++) {
        ret = _dehaze_SetMDehazeStrth(ctx_array.ctx[i], ctrl);
    }
    return ret;
}

XCamReturn rk_aiq_uapi2_getDehazeEnhanceStrth(const rk_aiq_sys_ctx_t* sys_ctx, adehaze_strength_t *ctrl)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    CHECK_USER_API_ENABLE2(sys_ctx);
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ADHAZ);
    RKAIQ_API_SMART_LOCK(sys_ctx);
	const rk_aiq_sys_ctx_t* _ctx = rk_aiq_user_api2_common_getSysCtx(sys_ctx);
    return _dehaze_GetMDehazeStrth(_ctx, ctrl);
}

XCamReturn
rk_aiq_user_api2_dehaze_SetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, dehaze_api_attrib_t* attr)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    CHECK_USER_API_ENABLE2(sys_ctx);
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ADHAZ);
    RKAIQ_API_SMART_LOCK(sys_ctx);

	rk_aiq_sys_ctx_array_t ctx_array = rk_aiq_user_api2_common_getSysCtxArray(sys_ctx);

	int type = RESULT_TYPE_DEHAZE_PARAM;
	int man_param_size = sizeof(dehaze_param_t);
	int aut_param_size = sizeof(dehaze_param_auto_t);

    for (int i = 0; i < ctx_array.num; i++) {
        if (attr->opMode == RK_AIQ_OP_MODE_MANUAL || attr->opMode == RK_AIQ_OP_MODE_AUTO) {
            ret = rk_aiq_user_api2_common_processParams(ctx_array.ctx[i], true,
                    &attr->opMode, &attr->en, &attr->bypass,
                    type, man_param_size, &attr->stMan, aut_param_size,  &attr->stAuto);
        } else {
            ret = XCAM_RETURN_ERROR_FAILED;
            LOGE_ADEHAZE("wrong mode %d !", attr->opMode);
        }
    }

    return ret;
}

XCamReturn
rk_aiq_user_api2_dehaze_GetAttrib(const rk_aiq_sys_ctx_t* sys_ctx, dehaze_api_attrib_t* attr)
{
    CHECK_USER_API_ENABLE2(sys_ctx);
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ADHAZ);
    RKAIQ_API_SMART_LOCK(sys_ctx);

	const rk_aiq_sys_ctx_t* ctx = rk_aiq_user_api2_common_getSysCtx(sys_ctx);
	int type = RESULT_TYPE_DEHAZE_PARAM;
	int man_param_size = sizeof(dehaze_param_t);
	int aut_param_size = sizeof(dehaze_param_auto_t);

    return rk_aiq_user_api2_common_processParams(ctx, false,
				&attr->opMode, &attr->en, &attr->bypass,
				type, man_param_size, &attr->stMan, aut_param_size,  &attr->stAuto);
}

XCamReturn
rk_aiq_user_api2_dehaze_QueryStatus(const rk_aiq_sys_ctx_t* sys_ctx, dehaze_status_t* status)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    CHECK_USER_API_ENABLE2(sys_ctx);
    CHECK_USER_API_ENABLE(RK_AIQ_ALGO_TYPE_ADHAZ);
    RKAIQ_API_SMART_LOCK(sys_ctx);
	const rk_aiq_sys_ctx_t* ctx = rk_aiq_user_api2_common_getSysCtx(sys_ctx);
	AiqAlgoHandler_t* algo_handle =
		ctx->_analyzer->mAlgoHandleMaps[RK_AIQ_ALGO_TYPE_ADHAZ];
	if (algo_handle) {
		ret = AiqAlgoHandler_queryStatus_common(algo_handle,
				&status->opMode, &status->en, &status->bypass, &status->stMan);
	}
	return ret;
}

RKAIQ_END_DECLARE
