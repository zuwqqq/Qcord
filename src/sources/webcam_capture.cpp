#include "sources/webcam_capture.h"
#include <iostream>

namespace Qcord {

WebcamCapture::WebcamCapture(const std::string& device_path)
    : device_path_(device_path), width_(640), height_(480), fps_(30), active_(false)
#ifdef _WIN32
    , dshow_graph_(nullptr)
#elif __APPLE__
    , av_capture_session_(nullptr)
#else
    , v4l2_fd_(-1)
#endif
{
    if (device_path_.empty()) {
#ifdef _WIN32
        device_path_ = "";
#elif __APPLE__
        device_path_ = "";
#else
        device_path_ = "/dev/video0";
#endif
    }
}

WebcamCapture::~WebcamCapture() {
    stop();
}

bool WebcamCapture::start() {
#ifdef _WIN32
    // Windows: Use DirectShow
    std::cout << "Starting Windows webcam capture..." << std::endl;
    // TODO: Initialize DirectShow graph for camera
    active_ = true;
    return true;
#elif __APPLE__
    // macOS: Use AVFoundation
    std::cout << "Starting macOS webcam capture..." << std::endl;
    // TODO: Initialize AVCaptureSession
    active_ = true;
    return true;
#else
    // Linux: Use V4L2
    std::cout << "Starting Linux webcam capture (" << device_path_ << ")..." << std::endl;
    // TODO: Open V4L2 device and start capture
    active_ = true;
    return true;
#endif
}

void WebcamCapture::stop() {
    if (!active_) return;

#ifdef _WIN32
    if (dshow_graph_) {
        // Release DirectShow resources
        dshow_graph_ = nullptr;
    }
#elif __APPLE__
    if (av_capture_session_) {
        // Stop AVCaptureSession
        av_capture_session_ = nullptr;
    }
#else
    if (v4l2_fd_ >= 0) {
        // Close V4L2 device
        // close(v4l2_fd_);
        v4l2_fd_ = -1;
    }
#endif

    active_ = false;
    std::cout << "Webcam capture stopped" << std::endl;
}

bool WebcamCapture::getFrame(uint8_t* buffer, int& width, int& height) {
    if (!active_) return false;
    
    width = width_;
    height = height_;
    // TODO: Read frame from camera
    return true;
}

void WebcamCapture::setResolution(int width, int height) {
    width_ = width;
    height_ = height;
}

void WebcamCapture::setFramerate(int fps) {
    fps_ = fps;
}

} // namespace Qcord