#pragma once

#include "core/source.h"
#include <cstdint>
#include <vector>

namespace Qcord {

/**
 * @class ScreenCapture
 * @brief Captures screen/display output
 */
class ScreenCapture : public Source {
public:
    ScreenCapture(int display_index = 0);
    ~ScreenCapture() override;

    std::string getName() const override { return "Screen Capture"; }
    Type getType() const override { return Type::SCREEN_CAPTURE; }
    bool start() override;
    void stop() override;
    bool isActive() const override { return active_; }

    bool getFrame(uint8_t* buffer, int& width, int& height);
    void setResolution(int width, int height);

private:
    int display_index_;
    int width_;
    int height_;
    bool active_;
    
#ifdef _WIN32
    void* hdc_;
    void* hdc_mem_;
    void* hbitmap_;
#elif __APPLE__
    void* cg_display_id_;
#else // Linux
    int x11_display_;
#endif
};

} // namespace Qcord