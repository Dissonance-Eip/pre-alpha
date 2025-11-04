#pragma once

#include <string>
#include <vector>
#include <cstdint>

class AudioPlayer {
public:
    AudioPlayer() = default;
    ~AudioPlayer();

    // Start async playback of interleaved 16-bit PCM samples; returns true on success
    bool playFromBuffer(const std::vector<int16_t>& interleaved, unsigned sampleRate, unsigned channels, const std::string& tempAlias = "mywav");

    // Pause/resume/stop
    void pause();
    void resume();
    void stop();

    // Polling helper - returns true if playing
    [[nodiscard]] bool isPlaying() const;

private:
    // Platform-specific implementation details hidden in cpp
    struct Impl;
    Impl* impl = nullptr;
};
