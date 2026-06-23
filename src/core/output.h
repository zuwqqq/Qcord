#pragma once

#include <string>
#include <memory>

namespace Qcord {

/**
 * @class Output
 * @brief Abstract base class for streaming/recording outputs
 */
class Output {
public:
    enum class Type {
        TWITCH,
        YOUTUBE,
        TELEGRAM,
        LOCAL_FILE,
        CUSTOM_RTMP
    };

    virtual ~Output() = default;

    virtual std::string getName() const = 0;
    virtual Type getType() const = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    virtual bool sendFrame(const void* data, size_t size) = 0;
    virtual std::string getStatus() const = 0;
};

} // namespace Qcord