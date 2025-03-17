//
// Created by luca on 17/03/2025.
//

#ifndef WAVHEADER_H
    #define WAVHEADER_H

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <stdexcept>

class WavHeader {
public:
    WavHeader() = default;

    void readFromFile(std::ifstream& file) {
        if (!file.read(reinterpret_cast<char*>(this), sizeof(WavHeader))) {
            throw std::runtime_error("Failed to read WAV header");
        }
    }

    // Getters
    [[nodiscard]] std::string getRiff() const { return {riff.data(), riff.size()}; }
    [[nodiscard]] uint32_t getChunkSize() const { return chunkSize; }
    [[nodiscard]] std::string getWave() const { return {wave.data(), wave.size()}; }
    [[nodiscard]] std::string getFmt() const { return {fmt.data(), fmt.size()}; }
    [[nodiscard]] uint32_t getSubchunk1Size() const { return subchunk1Size; }
    [[nodiscard]] uint16_t getAudioFormat() const { return audioFormat; }
    [[nodiscard]] uint16_t getNumChannels() const { return numChannels; }
    [[nodiscard]] uint32_t getSampleRate() const { return sampleRate; }
    [[nodiscard]] uint32_t getByteRate() const { return byteRate; }
    [[nodiscard]] uint16_t getBlockAlign() const { return blockAlign; }
    [[nodiscard]] uint16_t getBitsPerSample() const { return bitsPerSample; }
    [[nodiscard]] std::string getData() const { return {data.data(), data.size()}; }
    [[nodiscard]] uint32_t getSubchunk2Size() const { return subchunk2Size; }

private:
    std::array<char, 4> riff;        // "RIFF"
    uint32_t chunkSize;              // Size of the entire file in bytes minus 8 bytes
    std::array<char, 4> wave;        // "WAVE"
    std::array<char, 4> fmt;         // "fmt "
    uint32_t subchunk1Size;          // Size of the fmt chunk
    uint16_t audioFormat;            // Audio format (1 for PCM)
    uint16_t numChannels;            // Number of channels
    uint32_t sampleRate;             // Sample rate
    uint32_t byteRate;               // Byte rate
    uint16_t blockAlign;             // Block align
    uint16_t bitsPerSample;          // Bits per sample
    std::array<char, 4> data;        // "data"
    uint32_t subchunk2Size;          // Size of the data chunk
};

#endif //WAVHEADER_H
