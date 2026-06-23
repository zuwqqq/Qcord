#pragma once

#include "core/output.h"
#include <string>

namespace Qcord {

/**
 * @class TelegramOutput
 * @brief Output module for Telegram video sending
 */
class TelegramOutput : public Output {
public:
    TelegramOutput();
    ~TelegramOutput() override;

    std::string getName() const override { return "Telegram"; }
    Type getType() const override { return Type::TELEGRAM; }

    void setBotToken(const std::string& token);
    void setChatId(const std::string& chat_id);
    void setChannelId(const std::string& channel_id);

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    bool sendFrame(const void* data, size_t size) override;
    std::string getStatus() const override;

private:
    std::string bot_token_;
    std::string chat_id_;
    std::string channel_id_;
    bool connected_;
};

} // namespace Qcord