/*
 * rk_aiq_types_a3dlut_hw.h
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

#ifndef _RK_AIQ_TYPES_A3DLUT_HW_H_
#define _RK_AIQ_TYPES_A3DLUT_HW_H_
#include "common/rk_aiq_comm.h"
#define LUT3D_LUT_GRID_NUM 9
#define LUT3D_LUT_WSIZE   (LUT3D_LUT_GRID_NUM*LUT3D_LUT_GRID_NUM*LUT3D_LUT_GRID_NUM)
RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_lut3d_hw_tbl_s {
    unsigned short look_up_table_r[LUT3D_LUT_WSIZE];//10bit
    unsigned short look_up_table_g[LUT3D_LUT_WSIZE];//12bit
    unsigned short look_up_table_b[LUT3D_LUT_WSIZE];//10bit
} rk_aiq_lut3d_hw_tbl_t;

typedef struct rk_aiq_lut3d_cfg_s {
    bool enable;
    bool bypass_en;//bypass_en = ! enable
    unsigned int lut3d_lut_wsize;
    rk_aiq_lut3d_hw_tbl_t tbl;
} rk_aiq_lut3d_cfg_t;

RKAIQ_END_DECLARE

#endif
