#pragma once

#include <vector>
#include <cstdint>

class Equalizer {
public:
    // sampleRate in Hz, channels number of channels
    Equalizer(unsigned sampleRate, unsigned numChannels);

    // Set gains in dB
    void setLowGainDb(float db);
    void setMidGainDb(float db);
    void setHighGainDb(float db);

    [[nodiscard]] float getLowGainDb() const;
    [[nodiscard]] float getMidGainDb() const;
    [[nodiscard]] float getHighGainDb() const;

    // Process interleaved 16-bit PCM samples (in -> out)
    void process(const std::vector<int16_t>& in, std::vector<int16_t>& out);

private:
    unsigned sampleRate;
    unsigned channels;

    float lowGainLinear = 1.0f;
    float midGainLinear = 1.0f;
    float highGainLinear = 1.0f;

    // filter state per channel
    std::vector<float> prevLow;
    std::vector<float> prevLowForHP; // previous low used for high-pass approx

    float lowCutoff = 300.0f;   // Hz
    float highCutoff = 3000.0f; // Hz

    float alphaLow = 0.0f;
    float alphaHigh = 0.0f;

    void updateCoefficients();
};
