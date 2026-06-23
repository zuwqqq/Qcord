#pragma once

#include <string>
#include <memory>

namespace Qcord {

/**
 * @class Source
 * @brief Abstract base class for media sources (screen capture, webcam, etc)
 */
class Source {
public:
    enum class Type {
        SCREEN_CAPTURE,
        WINDOW_CAPTURE,
        WEBCAM,
        AUDIO_INPUT,
        IMAGE,
        MEDIA_FILE
    };

    virtual ~Source() = default;

    virtual std::string getName() const = 0;
    virtual Type getType() const = 0;
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual bool isActive() const = 0;
};

} // namespace Qcord