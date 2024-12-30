/*
* merge_types_prvt.h

* for rockchip v2.0.0
*
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
/* for rockchip v2.0.0*/

#ifndef __RKAIQ_TYPES_MERGE_ALGO_PRVT_H__
#define __RKAIQ_TYPES_MERGE_ALGO_PRVT_H__

/**
 * @file merge_types_prvt.h
 *
 * @brief
 *
 *****************************************************************************/
/**
 * @page module_name_page Module Name
 * Describe here what this module does.
 *
 * For a detailed list of functions and implementation detail refer to:
 * - @ref module_name
 *
 * @defgroup merge Auto merge
 * @{
 *
 */
#include "include/merge_algo_api.h"
#include "RkAiqCalibDbTypes.h"
#include "RkAiqCalibDbTypesV2.h"
#include "RkAiqCalibDbV2Helper.h"
#include "xcam_log.h"

#define INIT_CALC_PARAMS_NUM  (2)
#define LONG_FRAME_MODE_RATIO (1.0f)
#define RATIO_DEFAULT         (1.0f)

#define DEFAULT_RECALCULATE_DELTA_ISO (0.01)

// typedef enum FrameNumber_e {
//     LINEAR_NUM = 1,
//     HDR_2X_NUM = 2,
//     HDR_3X_NUM = 3,
//     SENSOR_MGE = 4,
//     HDR_NUM_MAX
// } FrameNumber_t;

typedef struct mergeExpoData_s {
    bool LongFrmMode;
    float EnvLv;
    float ISO;

    float LExpo;
    float MExpo;
    float SExpo;
    float SGain;
    float MGain;
    float RatioLS;
    float RatioLM;
} mergeExpoData_t;

typedef struct mergeCurrCtrlData_s {
    int MergeMode;
    float MoveCoef;
    mergeExpoData_t ExpoData;
} mergeCurrCtrlData_t;

typedef struct mergeCurrData_s {
    mergeCurrCtrlData_t CtrlData;
    mge_params_dyn_t dyn;
} mergeCurrData_t;

typedef struct mergeNextCtrlData_s {
    mergeExpoData_t ExpoData;
    float MoveCoef;
    float MergeOEDamp;
    float MergeMDDamp;
} mergeNextCtrlData_t;

typedef struct mergeContext_s {
    const RkAiqAlgoCom_prepare_t* prepare_params;
    mge_api_attrib_t* merge_attrib;
    int iso;
    bool isReCal_;
    FrameNumber_t FrameNumber;
    bool isCapture;
    uint32_t FrameID;
    mergeCurrData_t CurrData;
    mergeNextCtrlData_t NextCtrlData;
} MergeContext_t;

XCamReturn MergeSelectParam(MergeContext_t *pMergeCtx, mge_param_t* out, int iso);

#endif//__RKAIQ_TYPES_MERGE_ALGO_PRVT_H__