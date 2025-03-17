//
// Created by noe on 16/03/2025.
//

#ifndef WAVPARSER_H
#define WAVPARSER_H

#include <string>
#include <cstdint>

struct WavHeader {
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

class Parser {
public:
    Parser(const std::string& filename);
    bool isValid() const;
    void printMetadata() const;
    void printAudioData() const;

private:
    WavHeader header;
    bool valid = false;
};

#endif // WAVPARSER_H