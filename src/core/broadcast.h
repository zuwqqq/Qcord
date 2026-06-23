#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Qcord {

class Source;
class Encoder;
class Output;
class Filter;

/**
 * @class Broadcast
 * @brief Main broadcaster class handling sources, encoding, and outputs
 */
class Broadcast {
public:
    Broadcast();
    ~Broadcast();

    void start();
    void stop();
    bool isRunning() const { return running_; }

    void addSource(std::shared_ptr<Source> source);
    void removeSource(std::shared_ptr<Source> source);
    const std::vector<std::shared_ptr<Source>>& getSources() const { return sources_; }

    void setEncoder(std::shared_ptr<Encoder> encoder);
    std::shared_ptr<Encoder> getEncoder() const { return encoder_; }

    void addOutput(std::shared_ptr<Output> output);
    void removeOutput(std::shared_ptr<Output> output);
    const std::vector<std::shared_ptr<Output>>& getOutputs() const { return outputs_; }

    void addFilter(std::shared_ptr<Filter> filter);
    void removeFilter(std::shared_ptr<Filter> filter);

    void setResolution(int width, int height);
    void setFrameRate(int fps);
    void setBitrate(int kbps);

private:
    bool running_;
    std::vector<std::shared_ptr<Source>> sources_;
    std::vector<std::shared_ptr<Output>> outputs_;
    std::vector<std::shared_ptr<Filter>> filters_;
    std::shared_ptr<Encoder> encoder_;

    int width_;
    int height_;
    int fps_;
    int bitrate_kbps_;
};

} // namespace Qcord