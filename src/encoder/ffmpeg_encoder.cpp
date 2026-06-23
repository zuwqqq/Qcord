#include "encoder/ffmpeg_encoder.h"
#include <iostream>
#include <cstring>

namespace Qcord {

FFmpegEncoder::FFmpegEncoder()
    : video_codec_ctx_(nullptr), video_frame_(nullptr), sws_ctx_(nullptr),
      video_width_(0), video_height_(0), video_fps_(30), video_bitrate_(6000),
      frame_count_(0),
      audio_codec_ctx_(nullptr), audio_frame_(nullptr), swr_ctx_(nullptr),
      audio_sample_rate_(48000), audio_channels_(2), audio_bitrate_(128),
      audio_pts_(0),
      packet_(nullptr) {
    packet_ = av_packet_alloc();
}

FFmpegEncoder::~FFmpegEncoder() {
    reset();
    if (packet_) {
        av_packet_free(&packet_);
    }
}

const char* FFmpegEncoder::codecToString(VideoCodec codec) {
    switch (codec) {
        case VideoCodec::H264:  return "h264";
        case VideoCodec::H265:  return "hevc";
        case VideoCodec::VP9:   return "vp9";
        default:                return "h264";
    }
}

const char* FFmpegEncoder::codecToString(AudioCodec codec) {
    switch (codec) {
        case AudioCodec::AAC:  return "aac";
        case AudioCodec::OPUS: return "opus";
        case AudioCodec::MP3:  return "libmp3lame";
        default:               return "aac";
    }
}

bool FFmpegEncoder::initVideo(int width, int height, int fps, int bitrate_kbps, VideoCodec codec) {
    video_width_ = width;
    video_height_ = height;
    video_fps_ = fps;
    video_bitrate_ = bitrate_kbps;

    const AVCodec* h264_codec = avcodec_find_encoder_by_name(codecToString(codec));
    if (!h264_codec) {
        std::cerr << "Could not find " << codecToString(codec) << " encoder" << std::endl;
        return false;
    }

    video_codec_ctx_ = avcodec_alloc_context3(h264_codec);
    if (!video_codec_ctx_) {
        std::cerr << "Could not allocate video codec context" << std::endl;
        return false;
    }

    video_codec_ctx_->bit_rate = bitrate_kbps * 1000;
    video_codec_ctx_->width = width;
    video_codec_ctx_->height = height;
    video_codec_ctx_->time_base = {1, fps};
    video_codec_ctx_->gop_size = fps;  // Keyframe every second
    video_codec_ctx_->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(video_codec_ctx_, h264_codec, nullptr) < 0) {
        std::cerr << "Could not open video codec" << std::endl;
        avcodec_free_context(&video_codec_ctx_);
        return false;
    }

    video_frame_ = av_frame_alloc();
    if (!video_frame_) {
        std::cerr << "Could not allocate video frame" << std::endl;
        avcodec_close(video_codec_ctx_);
        avcodec_free_context(&video_codec_ctx_);
        return false;
    }

    video_frame_->format = AV_PIX_FMT_YUV420P;
    video_frame_->width = width;
    video_frame_->height = height;
    av_frame_get_buffer(video_frame_, 0);

    // Create software scaler context for RGB to YUV420P conversion
    sws_ctx_ = sws_getContext(width, height, AV_PIX_FMT_RGB24,
                              width, height, AV_PIX_FMT_YUV420P,
                              SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!sws_ctx_) {
        std::cerr << "Could not create SWS context" << std::endl;
        return false;
    }

    std::cout << "Video encoder initialized: " << width << "x" << height << " @ " << fps << "fps, " 
              << bitrate_kbps << "kbps" << std::endl;
    return true;
}

bool FFmpegEncoder::initAudio(int sample_rate, int channels, int bitrate_kbps, AudioCodec codec) {
    audio_sample_rate_ = sample_rate;
    audio_channels_ = channels;
    audio_bitrate_ = bitrate_kbps;

    const AVCodec* audio_codec = avcodec_find_encoder_by_name(codecToString(codec));
    if (!audio_codec) {
        std::cerr << "Could not find " << codecToString(codec) << " encoder" << std::endl;
        return false;
    }

    audio_codec_ctx_ = avcodec_alloc_context3(audio_codec);
    if (!audio_codec_ctx_) {
        std::cerr << "Could not allocate audio codec context" << std::endl;
        return false;
    }

    audio_codec_ctx_->bit_rate = bitrate_kbps * 1000;
    audio_codec_ctx_->sample_rate = sample_rate;
    audio_codec_ctx_->channels = channels;
    audio_codec_ctx_->channel_layout = av_get_default_channel_layout(channels);
    audio_codec_ctx_->sample_fmt = audio_codec->sample_fmts[0];
    audio_codec_ctx_->time_base = {1, sample_rate};

    if (avcodec_open2(audio_codec_ctx_, audio_codec, nullptr) < 0) {
        std::cerr << "Could not open audio codec" << std::endl;
        avcodec_free_context(&audio_codec_ctx_);
        return false;
    }

    audio_frame_ = av_frame_alloc();
    if (!audio_frame_) {
        std::cerr << "Could not allocate audio frame" << std::endl;
        avcodec_close(audio_codec_ctx_);
        avcodec_free_context(&audio_codec_ctx_);
        return false;
    }

    audio_frame_->nb_samples = audio_codec_ctx_->frame_size;
    audio_frame_->format = audio_codec_ctx_->sample_fmt;
    audio_frame_->channel_layout = audio_codec_ctx_->channel_layout;
    av_frame_get_buffer(audio_frame_, 0);

    std::cout << "Audio encoder initialized: " << sample_rate << "Hz, " << channels 
              << "ch, " << bitrate_kbps << "kbps" << std::endl;
    return true;
}

bool FFmpegEncoder::encodeVideoFrame(const uint8_t* frame_data, uint8_t** encoded_data, int* encoded_size, int64_t* pts) {
    if (!video_codec_ctx_ || !video_frame_) return false;

    // Convert RGB to YUV420P
    const uint8_t* srcSlice[] = {frame_data};
    int srcStride[] = {video_width_ * 3};  // RGB24
    uint8_t* dstSlice[] = {video_frame_->data[0], video_frame_->data[1], video_frame_->data[2]};
    int dstStride[] = {video_frame_->linesize[0], video_frame_->linesize[1], video_frame_->linesize[2]};
    
    sws_scale(sws_ctx_, srcSlice, srcStride, 0, video_height_, dstSlice, dstStride);

    video_frame_->pts = frame_count_++;

    if (avcodec_send_frame(video_codec_ctx_, video_frame_) < 0) {
        std::cerr << "Error sending video frame to encoder" << std::endl;
        return false;
    }

    if (avcodec_receive_packet(video_codec_ctx_, packet_) == 0) {
        *encoded_data = packet_->data;
        *encoded_size = packet_->size;
        *pts = packet_->pts;
        return true;
    }

    return false;
}

bool FFmpegEncoder::encodeAudioFrame(const uint8_t* audio_data, int sample_count, uint8_t** encoded_data, int* encoded_size) {
    if (!audio_codec_ctx_ || !audio_frame_) return false;

    // Copy audio data into frame
    std::memcpy(audio_frame_->data[0], audio_data, sample_count * audio_channels_ * sizeof(int16_t));
    audio_frame_->nb_samples = sample_count;
    audio_frame_->pts = audio_pts_;
    audio_pts_ += sample_count;

    if (avcodec_send_frame(audio_codec_ctx_, audio_frame_) < 0) {
        std::cerr << "Error sending audio frame to encoder" << std::endl;
        return false;
    }

    if (avcodec_receive_packet(audio_codec_ctx_, packet_) == 0) {
        *encoded_data = packet_->data;
        *encoded_size = packet_->size;
        return true;
    }

    return false;
}

void FFmpegEncoder::reset() {
    if (video_frame_) {
        av_frame_free(&video_frame_);
        video_frame_ = nullptr;
    }
    if (video_codec_ctx_) {
        avcodec_close(video_codec_ctx_);
        avcodec_free_context(&video_codec_ctx_);
        video_codec_ctx_ = nullptr;
    }
    if (sws_ctx_) {
        sws_freeContext(sws_ctx_);
        sws_ctx_ = nullptr;
    }

    if (audio_frame_) {
        av_frame_free(&audio_frame_);
        audio_frame_ = nullptr;
    }
    if (audio_codec_ctx_) {
        avcodec_close(audio_codec_ctx_);
        avcodec_free_context(&audio_codec_ctx_);
        audio_codec_ctx_ = nullptr;
    }
    if (swr_ctx_) {
        swr_free(&swr_ctx_);
        swr_ctx_ = nullptr;
    }
}

} // namespace Qcord