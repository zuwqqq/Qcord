#include "outputs/twitch/twitch_output.h"

namespace Qcord {

TwitchOutput::TwitchOutput() : connected_(false) {
}

TwitchOutput::~TwitchOutput() {
    disconnect();
}

void TwitchOutput::setServerUrl(const std::string& url) {
    server_url_ = url;
}

void TwitchOutput::setStreamKey(const std::string& key) {
    stream_key_ = key;
}

void TwitchOutput::setChannelName(const std::string& channel) {
    channel_name_ = channel;
}

bool TwitchOutput::connect() {
    // TODO: Connect to Twitch RTMP server
    // TODO: Use FFmpeg to establish connection
    connected_ = true;
    return true;
}

void TwitchOutput::disconnect() {
    // TODO: Disconnect from Twitch
    connected_ = false;
}

bool TwitchOutput::isConnected() const {
    return connected_;
}

bool TwitchOutput::sendFrame(const void* data, size_t size) {
    // TODO: Send encoded frame to Twitch
    return true;
}

std::string TwitchOutput::getStatus() const {
    return connected_ ? "Connected to Twitch" : "Disconnected";
}

} // namespace Qcord