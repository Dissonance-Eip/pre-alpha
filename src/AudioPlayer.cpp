#include "AudioPlayer.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
#endif

struct AudioPlayer::Impl {
#ifdef _WIN32
    std::string alias;
    std::string tempFilename;
    bool playing = false;
    bool paused = false;
#endif
};

AudioPlayer::~AudioPlayer() {
    stop();
    delete impl;
}

bool AudioPlayer::playFromBuffer(const std::vector<int16_t>& interleaved, unsigned sampleRate, unsigned channels, const std::string& tempAlias) {
#ifdef _WIN32
    if (impl == nullptr) impl = new Impl();

    // Write to a temporary WAV file (very small WAV header)
    std::string tmp = "temp_play.wav";
    std::ofstream out(tmp, std::ios::binary);
    if (!out) return false;

    uint32_t byteRate = sampleRate * channels * 2;
    uint16_t blockAlign = channels * 2;
    uint32_t dataSize = static_cast<uint32_t>(interleaved.size() * sizeof(int16_t));

    // RIFF header
    out.write("RIFF", 4);
    uint32_t chunkSize = 36 + dataSize;
    out.write(reinterpret_cast<const char*>(&chunkSize), 4);
    out.write("WAVE", 4);

    // fmt chunk
    out.write("fmt ", 4);
    uint32_t subchunk1Size = 16;
    out.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    uint16_t audioFormat = 1;
    out.write(reinterpret_cast<const char*>(&audioFormat), 2);
    out.write(reinterpret_cast<const char*>(&channels), 2);
    out.write(reinterpret_cast<const char*>(&sampleRate), 4);
    out.write(reinterpret_cast<const char*>(&byteRate), 4);
    out.write(reinterpret_cast<const char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 16;
    out.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // data chunk
    out.write("data", 4);
    out.write(reinterpret_cast<const char*>(&dataSize), 4);
    out.write(reinterpret_cast<const char*>(interleaved.data()), dataSize);
    out.close();

    // Open via MCI
    std::string openCmd = "open \"" + tmp + "\" type waveaudio alias " + tempAlias;
    impl->alias = tempAlias;
    impl->tempFilename = tmp;

    if (mciSendStringA(openCmd.c_str(), nullptr, 0, nullptr) != 0) {
        return false;
    }
    if (mciSendStringA(("play " + tempAlias).c_str(), nullptr, 0, nullptr) != 0) {
        mciSendStringA(("close " + tempAlias).c_str(), nullptr, 0, nullptr);
        return false;
    }
    impl->playing = true;
    impl->paused = false;
    return true;
#else
    (void)interleaved; (void)sampleRate; (void)channels; (void)tempAlias;
    return false;
#endif
}

void AudioPlayer::pause() {
#ifdef _WIN32
    if (impl && impl->playing && !impl->paused) {
        mciSendStringA((std::string("pause ") + impl->alias).c_str(), nullptr, 0, nullptr);
        impl->paused = true;
    }
#endif
}

void AudioPlayer::resume() {
#ifdef _WIN32
    if (impl && impl->playing && impl->paused) {
        mciSendStringA((std::string("play ") + impl->alias).c_str(), nullptr, 0, nullptr);
        impl->paused = false;
    }
#endif
}

void AudioPlayer::stop() {
#ifdef _WIN32
    if (impl && impl->playing) {
        mciSendStringA((std::string("stop ") + impl->alias).c_str(), nullptr, 0, nullptr);
        mciSendStringA((std::string("close ") + impl->alias).c_str(), nullptr, 0, nullptr);
        impl->playing = false;
        impl->paused = false;
        // remove temp file
        if (!impl->tempFilename.empty()) {
            std::remove(impl->tempFilename.c_str());
            impl->tempFilename.clear();
        }
    }
#endif
}

bool AudioPlayer::isPlaying() const {
#ifdef _WIN32
    return impl && impl->playing && !impl->paused;
#else
    return false;
#endif
}

