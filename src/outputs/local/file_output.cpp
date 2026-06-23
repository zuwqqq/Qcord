#include "outputs/local/file_output.h"
#include "encoder/ffmpeg_encoder.h"
#include <iostream>

namespace Qcord {

FileOutput::FileOutput()
    : file_path_("output.mp4"), video_codec_("h264"), audio_codec_("aac"), connected_(false) {
}

FileOutput::~FileOutput() {
    disconnect();
}

void FileOutput::setFilePath(const std::string& path) {
    file_path_ = path;
}

void FileOutput::setVideoCodec(const std::string& codec) {
    video_codec_ = codec;
}

void FileOutput::setAudioCodec(const std::string& codec) {
    audio_codec_ = codec;
}

bool FileOutput::connect() {
    encoder_ = std::make_shared<FFmpegEncoder>();
    if (!encoder_) return false;

    // Initialize encoders
    if (!encoder_->initVideo(1920, 1080, 30, 6000)) {
        std::cerr << "Failed to initialize video encoder" << std::endl;
        return false;
    }

    if (!encoder_->initAudio(48000, 2, 128)) {
        std::cerr << "Failed to initialize audio encoder" << std::endl;
        return false;
    }

    connected_ = true;
    std::cout << "File output initialized: " << file_path_ << std::endl;
    return true;
}

void FileOutput::disconnect() {
    if (encoder_) {
        encoder_.reset();
    }
    connected_ = false;
    std::cout << "File output disconnected" << std::endl;
}

bool FileOutput::sendFrame(const void* data, size_t size) {
    if (!connected_ || !encoder_) return false;
    
    // Encode frame
    // TODO: Write to MP4 container
    return true;
}

std::string FileOutput::getStatus() const {
    return connected_ ? ("Recording to " + file_path_) : "Not recording";
}

} // namespace Qcord