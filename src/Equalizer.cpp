#include "Equalizer.hpp"
#include <cmath>
#include <algorithm>
#include <cstdint>

Equalizer::Equalizer(unsigned sampleRate, unsigned numChannels)
    : sampleRate(sampleRate), channels(numChannels), alphaLow(0.0f), alphaHigh(0.0f) {
    prevLow.assign(channels, 0.0f);
    prevLowForHP.assign(channels, 0.0f);
    updateCoefficients();
}

void Equalizer::setLowGainDb(float db) {
    lowGainLinear = std::pow(10.0f, db / 20.0f);
}
void Equalizer::setMidGainDb(float db) {
    midGainLinear = std::pow(10.0f, db / 20.0f);
}
void Equalizer::setHighGainDb(float db) {
    highGainLinear = std::pow(10.0f, db / 20.0f);
}

float Equalizer::getLowGainDb() const { return 20.0f * std::log10(std::max(1e-8f, lowGainLinear)); }
float Equalizer::getMidGainDb() const { return 20.0f * std::log10(std::max(1e-8f, midGainLinear)); }
float Equalizer::getHighGainDb() const { return 20.0f * std::log10(std::max(1e-8f, highGainLinear)); }

void Equalizer::updateCoefficients() {
    // simple single-pole low-pass and high-pass approximation
    float dt = 1.0f / static_cast<float>(sampleRate);
    alphaLow = dt / (dt + 1.0f / (2.0f * 3.14159265f * lowCutoff));
    alphaHigh = dt / (dt + 1.0f / (2.0f * 3.14159265f * highCutoff));
}

void Equalizer::process(const std::vector<int16_t>& in, std::vector<int16_t>& out) {
    out.resize(in.size());
    if (channels == 0) return;

    const size_t frames = in.size() / channels;

    for (size_t f = 0; f < frames; ++f) {
        for (unsigned ch = 0; ch < channels; ++ch) {
            size_t idx = f * channels + ch;
            float sample = static_cast<float>(in[idx]) / 32768.0f;

            // low band via low-pass
            float low = prevLow[ch] + alphaLow * (sample - prevLow[ch]);
            prevLow[ch] = low;

            // high band via high-pass (sample - low-pass)
            float high = sample - (prevLowForHP[ch] + alphaHigh * (sample - prevLowForHP[ch]));
            prevLowForHP[ch] += alphaHigh * (sample - prevLowForHP[ch]);

            float mid = sample - low - high;

            float processed = low * lowGainLinear + mid * midGainLinear + high * highGainLinear;

            // clip
            processed = std::clamp(processed, -1.0f, 1.0f);
            out[idx] = static_cast<int16_t>(processed * 32767.0f);
        }
    }
}
