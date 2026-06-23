#pragma once

#include <string>

namespace Qcord {

/**
 * @class Encoder
 * @brief Handles video and audio encoding with configurable parameters
 */
class Encoder {
public:
    enum class VideoCodec {
        H264,
        H265,
        VP9,
        AV1
    };

    enum class AudioCodec {
        AAC,
        OPUS,
        FLAC
    };

    Encoder();
    ~Encoder();

    void setVideoCodec(VideoCodec codec);
    void setVideoBitrate(int kbps);
    void setResolution(int width, int height);
    void setFramerate(int fps);

    void setAudioCodec(AudioCodec codec);
    void setAudioBitrate(int kbps);
    void setAudioSampleRate(int hz);
    void setAudioChannels(int channels);

    void setQuality(int quality);

    bool initialize();
    void release();
};

} // namespace Qcord