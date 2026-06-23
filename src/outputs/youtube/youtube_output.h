#pragma once

#include "core/output.h"
#include <string>

namespace Qcord {

/**
 * @class YouTubeOutput
 * @brief Output module for YouTube Live streaming
 */
class YouTubeOutput : public Output {
public:
    YouTubeOutput();
    ~YouTubeOutput() override;

    std::string getName() const override { return "YouTube Live"; }
    Type getType() const override { return Type::YOUTUBE; }

    void setServerUrl(const std::string& url);
    void setStreamKey(const std::string& key);
    void setRefreshToken(const std::string& token);

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendFrame(const void* data, size_t size) override;
    std::string getStatus() const override;

private:
    std::string server_url_;
    std::string stream_key_;
    std::string refresh_token_;
    bool connected_;
};

} // namespace Qcord