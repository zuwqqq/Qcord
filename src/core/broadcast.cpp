#include "core/broadcast.h"
#include "core/source.h"
#include "core/output.h"
#include "core/encoder.h"
#include "filters/filter.h"
#include <algorithm>

namespace Qcord {

Broadcast::Broadcast()
    : running_(false), width_(1920), height_(1080), fps_(30), bitrate_kbps_(6000) {
}

Broadcast::~Broadcast() {
    if (running_) {
        stop();
    }
}

void Broadcast::start() {
    if (running_) return;
    
    running_ = true;
    
    // Start all sources
    for (auto& source : sources_) {
        source->start();
    }
    
    // Initialize encoder
    if (encoder_) {
        encoder_->setResolution(width_, height_);
        encoder_->setFramerate(fps_);
        encoder_->setVideoBitrate(bitrate_kbps_);
        encoder_->initialize();
    }
    
    // Connect all outputs
    for (auto& output : outputs_) {
        output->connect();
    }
}

void Broadcast::stop() {
    if (!running_) return;
    
    running_ = false;
    
    // Disconnect outputs
    for (auto& output : outputs_) {
        output->disconnect();
    }
    
    // Stop encoder
    if (encoder_) {
        encoder_->release();
    }
    
    // Stop all sources
    for (auto& source : sources_) {
        source->stop();
    }
}

void Broadcast::addSource(std::shared_ptr<Source> source) {
    if (source) {
        sources_.push_back(source);
    }
}

void Broadcast::removeSource(std::shared_ptr<Source> source) {
    auto it = std::find(sources_.begin(), sources_.end(), source);
    if (it != sources_.end()) {
        (*it)->stop();
        sources_.erase(it);
    }
}

void Broadcast::setEncoder(std::shared_ptr<Encoder> encoder) {
    encoder_ = encoder;
}

void Broadcast::addOutput(std::shared_ptr<Output> output) {
    if (output) {
        outputs_.push_back(output);
    }
}

void Broadcast::removeOutput(std::shared_ptr<Output> output) {
    auto it = std::find(outputs_.begin(), outputs_.end(), output);
    if (it != outputs_.end()) {
        (*it)->disconnect();
        outputs_.erase(it);
    }
}

void Broadcast::addFilter(std::shared_ptr<Filter> filter) {
    if (filter) {
        filter->initialize();
        filters_.push_back(filter);
    }
}

void Broadcast::removeFilter(std::shared_ptr<Filter> filter) {
    auto it = std::find(filters_.begin(), filters_.end(), filter);
    if (it != filters_.end()) {
        filters_.erase(it);
    }
}

void Broadcast::setResolution(int width, int height) {
    width_ = width;
    height_ = height;
}

void Broadcast::setFrameRate(int fps) {
    fps_ = fps;
}

void Broadcast::setBitrate(int kbps) {
    bitrate_kbps_ = kbps;
}

} // namespace Qcord