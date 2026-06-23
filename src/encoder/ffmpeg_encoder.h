#pragma once

#include <cstdint>
#include <string>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>

#ifdef __cplusplus
}
#endif

namespace Qcord {

/**
 * @class FFmpegEncoder
 * @brief Wrapper around FFmpeg for video and audio encoding
 */
class FFmpegEncoder {
public:
    enum class VideoCodec {
        H264,
        H265,
        VP9
    };

    enum class AudioCodec {
        AAC,
        OPUS,
        MP3
    };

    FFmpegEncoder();
    ~FFmpegEncoder();

    // Video encoding
    bool initVideo(int width, int height, int fps, int bitrate_kbps, VideoCodec codec = VideoCodec::H264);
    bool encodeVideoFrame(const uint8_t* frame_data, uint8_t** encoded_data, int* encoded_size, int64_t* pts);
    
    // Audio encoding
    bool initAudio(int sample_rate, int channels, int bitrate_kbps, AudioCodec codec = AudioCodec::AAC);
    bool encodeAudioFrame(const uint8_t* audio_data, int sample_count, uint8_t** encoded_data, int* encoded_size);

    // Utility
    void reset();
    bool isInitialized() const { return video_codec_ctx_ != nullptr || audio_codec_ctx_ != nullptr; }

private:
    // Video encoding
    AVCodecContext* video_codec_ctx_;
    AVFrame* video_frame_;
    SwsContext* sws_ctx_;
    int video_width_, video_height_, video_fps_, video_bitrate_;
    int64_t frame_count_;

    // Audio encoding  
    AVCodecContext* audio_codec_ctx_;
    AVFrame* audio_frame_;
    SwrContext* swr_ctx_;
    int audio_sample_rate_, audio_channels_, audio_bitrate_;
    int64_t audio_pts_;

    // Packet buffer
    AVPacket* packet_;

    // Helpers
    const char* codecToString(VideoCodec codec);
    const char* codecToString(AudioCodec codec);
};

} // namespace Qcord