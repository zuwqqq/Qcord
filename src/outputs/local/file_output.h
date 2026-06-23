#pragma once

#include "core/output.h"
#include <string>
#include <memory>

namespace Qcord {

class FFmpegEncoder;

/**
 * @class FileOutput
 * @brief Output module for local file recording
 */
class FileOutput : public Output {
public:
    FileOutput();
    ~FileOutput() override;

    std::string getName() const override { return "Local Recording"; }
    Type getType() const override { return Type::LOCAL_FILE; }

    void setFilePath(const std::string& path);
    void setVideoCodec(const std::string& codec);
    void setAudioCodec(const std::string& codec);

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override { return connected_; }
    bool sendFrame(const void* data, size_t size) override;
    std::string getStatus() const override;

private:
    std::string file_path_;
    std::string video_codec_;
    std::string audio_codec_;
    bool connected_;
    std::shared_ptr<FFmpegEncoder> encoder_;
};

} // namespace Qcord