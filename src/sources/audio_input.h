#pragma once

#include "core/source.h"
#include <cstdint>
#include <string>

namespace Qcord {

/**
 * @class AudioInput
 * @brief Captures audio from microphone or system audio
 */
class AudioInput : public Source {
public:
    enum class AudioType {
        MICROPHONE,
        SYSTEM_AUDIO,
        MIXED
    };

    AudioInput(AudioType type = AudioType::MICROPHONE);
    ~AudioInput() override;

    std::string getName() const override { return "Audio Input"; }
    Type getType() const override { return Type::AUDIO_INPUT; }
    bool start() override;
    void stop() override;
    bool isActive() const override { return active_; }

    bool getAudioFrame(uint8_t* buffer, int& sample_count);
    void setAudioFormat(int sample_rate, int channels);

private:
    AudioType audio_type_;
    int sample_rate_;
    int channels_;
    bool active_;
    
#ifdef _WIN32
    void* wasapi_client_;  // WASAPI audio client
#elif __APPLE__
    void* audio_unit_;
#else // Linux
    void* pa_stream_;  // PulseAudio stream
#endif
};

} // namespace Qcord