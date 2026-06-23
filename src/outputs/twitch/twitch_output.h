#pragma once

#include "core/output.h"
#include <string>

namespace Qcord {

/**
 * @class TwitchOutput
 * @brief Output module for Twitch streaming
 */
class TwitchOutput : public Output {
public:
    TwitchOutput();
    ~TwitchOutput() override;

    std::string getName() const override { return "Twitch"; }
    Type getType() const override { return Type::TWITCH; }

    void setServerUrl(const std::string& url);
    void setStreamKey(const std::string& key);
    void setChannelName(const std::string& channel);

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendFrame(const void* data, size_t size) override;
    std::string getStatus() const override;

private:
    std::string server_url_;
    std::string stream_key_;
    std::string channel_name_;
    bool connected_;
};

} // namespace Qcord