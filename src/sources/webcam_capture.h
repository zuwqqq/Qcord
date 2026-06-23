#pragma once

#include "core/source.h"
#include <cstdint>
#include <string>

namespace Qcord {

/**
 * @class WebcamCapture
 * @brief Captures video from webcam/camera devices
 */
class WebcamCapture : public Source {
public:
    WebcamCapture(const std::string& device_path = "");
    ~WebcamCapture() override;

    std::string getName() const override { return "Webcam"; }
    Type getType() const override { return Type::WEBCAM; }
    bool start() override;
    void stop() override;
    bool isActive() const override { return active_; }

    bool getFrame(uint8_t* buffer, int& width, int& height);
    void setResolution(int width, int height);
    void setFramerate(int fps);

private:
    std::string device_path_;
    int width_;
    int height_;
    int fps_;
    bool active_;
    
#ifdef _WIN32
    void* dshow_graph_;  // DirectShow graph
#elif __APPLE__
    void* av_capture_session_;
#else // Linux
    int v4l2_fd_;  // Video4Linux2 file descriptor
#endif
};

} // namespace Qcord