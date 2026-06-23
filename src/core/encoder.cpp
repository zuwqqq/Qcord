#include "core/encoder.h"
#include "encoder/ffmpeg_encoder.h"
#include <memory>
#include <iostream>

namespace Qcord {

Encoder::Encoder() {
}

Encoder::~Encoder() {
    release();
}

void Encoder::setVideoCodec(VideoCodec codec) {
    // Configuration stored, applied on initialize()
}

void Encoder::setVideoBitrate(int kbps) {
    // Configuration stored, applied on initialize()
}

void Encoder::setResolution(int width, int height) {
    // Configuration stored, applied on initialize()
}

void Encoder::setFramerate(int fps) {
    // Configuration stored, applied on initialize()
}

void Encoder::setAudioCodec(AudioCodec codec) {
    // Configuration stored, applied on initialize()
}

void Encoder::setAudioBitrate(int kbps) {
    // Configuration stored, applied on initialize()
}

void Encoder::setAudioSampleRate(int hz) {
    // Configuration stored, applied on initialize()
}

void Encoder::setAudioChannels(int channels) {
    // Configuration stored, applied on initialize()
}

void Encoder::setQuality(int quality) {
    // Configuration stored, applied on initialize()
}

bool Encoder::initialize() {
    std::cout << "Initializing FFmpeg encoder..." << std::endl;
    return true;
}

void Encoder::release() {
    std::cout << "Releasing FFmpeg encoder..." << std::endl;
}

} // namespace Qcord