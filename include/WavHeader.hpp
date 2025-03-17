//
// Created by luca on 17/03/2025.
//

#ifndef WAVHEADER_H
    #define WAVHEADER_H

#include <cstdint>
#include <algorithm>

class WavHeader {
public:
    WavHeader() = default;

    // Getters
    const char* getRiff() const { return riff; }
    uint32_t getChunkSize() const { return chunkSize; }
    const char* getWave() const { return wave; }
    const char* getFmt() const { return fmt; }
    uint32_t getSubchunk1Size() const { return subchunk1Size; }
    uint16_t getAudioFormat() const { return audioFormat; }
    uint16_t getNumChannels() const { return numChannels; }
    uint32_t getSampleRate() const { return sampleRate; }
    uint32_t getByteRate() const { return byteRate; }
    uint16_t getBlockAlign() const { return blockAlign; }
    uint16_t getBitsPerSample() const { return bitsPerSample; }
    const char* getData() const { return data; }
    uint32_t getSubchunk2Size() const { return subchunk2Size; }

    // Setters
    void setRiff(const char* value) { std::copy(value, value + 4, riff); }
    void setChunkSize(uint32_t value) { chunkSize = value; }
    void setWave(const char* value) { std::copy(value, value + 4, wave); }
    void setFmt(const char* value) { std::copy(value, value + 4, fmt); }
    void setSubchunk1Size(uint32_t value) { subchunk1Size = value; }
    void setAudioFormat(uint16_t value) { audioFormat = value; }
    void setNumChannels(uint16_t value) { numChannels = value; }
    void setSampleRate(uint32_t value) { sampleRate = value; }
    void setByteRate(uint32_t value) { byteRate = value; }
    void setBlockAlign(uint16_t value) { blockAlign = value; }
    void setBitsPerSample(uint16_t value) { bitsPerSample = value; }
    void setData(const char* value) { std::copy(value, value + 4, data); }
    void setSubchunk2Size(uint32_t value) { subchunk2Size = value; }

private:
    char riff[4];        // "RIFF"
    uint32_t chunkSize;  // Size of the entire file in bytes minus 8 bytes
    char wave[4];        // "WAVE"
    char fmt[4];         // "fmt "
    uint32_t subchunk1Size; // Size of the fmt chunk
    uint16_t audioFormat;   // Audio format (1 for PCM)
    uint16_t numChannels;   // Number of channels
    uint32_t sampleRate;    // Sample rate
    uint32_t byteRate;      // Byte rate
    uint16_t blockAlign;    // Block align
    uint16_t bitsPerSample; // Bits per sample
    char data[4];           // "data"
    uint32_t subchunk2Size; // Size of the data chunk
};

#endif //WAVHEADER_H
