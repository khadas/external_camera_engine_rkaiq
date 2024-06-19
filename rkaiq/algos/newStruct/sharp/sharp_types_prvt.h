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

#ifndef __SHARP_TYPES_PRIV_H__
#define __SHARP_TYPES_PRIV_H__

#include "include/sharp_algo_api.h"

typedef struct {
    sharp_api_attrib_t* sharp_attrib;
    int working_mode;
    int pre_iso;

    bool init_json;
    bool isReCal_;
    bool strength_en;
    float fStrength;
#if RKAIQ_HAVE_SHARP_V40
    texEst_api_attrib_t* texEst_attrib;
#endif
} SharpContext_t;

#define DEFAULT_RECALCULATE_DELTA_ISO (0.01)

#endif
