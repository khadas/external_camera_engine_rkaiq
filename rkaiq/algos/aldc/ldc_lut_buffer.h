/*
 *  Copyright (c) 2022 Rockchip Electronics Co.,Ltd
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
 * Author: Cody Xie <cody.xie@rock-chips.com>
 */
#ifndef ALGOS_ALDC_LUT_BUFFER_H
#define ALGOS_ALDC_LUT_BUFFER_H

#include <cstdint>

#include "common/rk_aiq_types_priv.h"

namespace RkCam {

enum class LdcLutBufferState {
    kInitial   = 0,
    kWait2Chip = 1,
    kChipInUse = 2,
};

struct LdcLutBufferConfig {
    uint32_t Width;
    uint32_t Height;
    uint32_t LutHCount;
    uint32_t LutVCount;
    uint32_t LutHStep;
    uint32_t LutVStep;
};

struct LdcLutBuffer {
    LdcLutBuffer() = delete;
    explicit LdcLutBuffer(const LdcLutBufferConfig& config)
        : State(LdcLutBufferState::kInitial), Config(config), Fd(-1), Size(0), Addr(nullptr) {}
    LdcLutBuffer(const LdcLutBufferConfig& config, const rk_aiq_lut_share_mem_info_t* mem_info)
        : Config(config) {
        State = static_cast<LdcLutBufferState>(*mem_info->state);
        Fd    = mem_info->fd;
        Addr  = mem_info->addr;
        Size  = mem_info->size;
    }
    LdcLutBuffer(const LdcLutBuffer&) = delete;
    LdcLutBuffer& operator=(const LdcLutBuffer&) = delete;

    LdcLutBufferState State;
    LdcLutBufferConfig Config;
    int Fd;
    int Size;
    void* Addr;
    int32_t mem_type;
};

class LdcLutBufferManager {
 public:
    LdcLutBufferManager() = delete;
    LdcLutBufferManager(const LdcLutBufferConfig& config, const isp_drv_share_mem_ops_t* mem_ops)
        : mem_ops_(mem_ops), mem_ctx_(nullptr), config_(config) {}
    LdcLutBufferManager(const LdcLutBufferManager&) = delete;
    LdcLutBufferManager& operator=(const LdcLutBufferManager&) = delete;
    ~LdcLutBufferManager() {
        // TODO(Cody)
        ReleaseHwBuffers(0);
        ReleaseHwBuffers(1);
    }

    void ImportHwBuffers(uint8_t isp_id, int32_t mem_type) {
        assert(mem_ops_ != nullptr);
        rk_aiq_share_mem_config_t hw_config;
        hw_config.mem_type                = static_cast<rk_aiq_drv_share_mem_type_t>(mem_type);
        hw_config.alloc_param.width       = config_.Width;
        hw_config.alloc_param.height      = config_.Height;
        hw_config.alloc_param.reserved[0] = 1;

        mem_ops_->alloc_mem(isp_id, (void*)(mem_ops_), &hw_config, &mem_ctx_);
    }

    void ReleaseHwBuffers(uint8_t isp_id) {
        if (mem_ctx_ != nullptr && mem_ops_ != nullptr) mem_ops_->release_mem(isp_id, mem_ctx_);
    }

    LdcLutBuffer* GetFreeHwBuffer(uint8_t isp_id) {
        if (mem_ops_ == nullptr || mem_ctx_ == nullptr) {
            return nullptr;
        }

        auto* mem_info = static_cast<rk_aiq_lut_share_mem_info_t*>(
            mem_ops_->get_free_item(isp_id, mem_ctx_));
        if (mem_info != nullptr) {
            *mem_info->state = MESH_BUF_WAIT2CHIP;
            auto* lut_buf = new LdcLutBuffer(config_, mem_info);
            if (lut_buf != nullptr) {
                return lut_buf;
            }
        }
        return nullptr;
    }

 private:
    const isp_drv_share_mem_ops_t* mem_ops_;
    void* mem_ctx_;
    LdcLutBufferConfig config_;
};

}  // namespace RkCam

#endif  // ALGOS_ALDC_LUT_BUFFER_H
