/*
 * SPDX-FileCopyrightText: 2026 CharaROM Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <compositionengine/UdfpsExtension.h>

#include <android-base/logging.h>
#include <display/drm/sde_drm.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

namespace {
using Clock = std::chrono::steady_clock;
constexpr auto kTouchTimeout = std::chrono::milliseconds(1500);

void setHbm(bool enabled) {
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        PLOG(ERROR) << "UDFPS: cannot open DRM card0";
        return;
    }
    panel_param_info param{};
    param.param_idx = PARAM_HBM;
    param.value = enabled ? 2 : 0;
    if (ioctl(fd, DRM_IOCTL_SET_PANEL_FEATURE, &param) < 0) {
        PLOG(ERROR) << "UDFPS: cannot set HBM to " << enabled;
    }
    close(fd);
}

// The touched layer has no guaranteed finger-up callback. Time out after its
// last composition so HBM cannot remain enabled indefinitely.
class HbmController {
  public:
    void touch() {
        std::lock_guard<std::mutex> lock(mutex_);
        deadline_ = Clock::now() + kTouchTimeout;
        if (!started_) {
            started_ = true;
            std::thread([this] { run(); }).detach();
        }
        if (!enabled_) {
            setHbm(true);
            enabled_ = true;
        }
        cv_.notify_one();
    }

  private:
    void run() {
        std::unique_lock<std::mutex> lock(mutex_);
        for (;;) {
            auto deadline = deadline_;
            if (cv_.wait_until(lock, deadline, [this, deadline] { return deadline_ != deadline; })) {
                continue;
            }
            if (enabled_) {
                setHbm(false);
                enabled_ = false;
            }
            cv_.wait(lock, [this] { return enabled_; });
        }
    }

    std::mutex mutex_;
    std::condition_variable cv_;
    Clock::time_point deadline_{};
    bool started_ = false;
    bool enabled_ = false;
};

HbmController& controller() {
    static auto* instance = new HbmController();
    return *instance;
}
}  // namespace

uint32_t getUdfpsDimZOrder(uint32_t z) {
    return z;
}

uint32_t getUdfpsZOrder(uint32_t z, bool touched) {
    if (touched) {
        controller().touch();
    }
    return z;
}

uint64_t getUdfpsUsageBits(uint64_t usageBits, bool) {
    return usageBits;
}
