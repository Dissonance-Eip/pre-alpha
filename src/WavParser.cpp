//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

Parser::Parser(const std::string& filename) : header(std::make_shared<WavHeader>()), filename(filename) {
    std::cout << "Opening file: " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::cout << "File opened successfully" << std::endl;
    header->readFromFile(file);
    valid = true;
}

void Parser::printMetadata() const {
    if (valid) {
        std::cout << "Chunk size: " << header->getChunkSize() << std::endl;
        std::cout << "Audio format: " << header->getAudioFormat() << std::endl;
        std::cout << "Number of channels: " << header->getNumChannels() << std::endl;
        std::cout << "Sample rate: " << header->getSampleRate() << std::endl;
        std::cout << "Byte rate: " << header->getByteRate() << std::endl;
        std::cout << "Block align: " << header->getBlockAlign() << std::endl;
        std::cout << "Bits per sample: " << header->getBitsPerSample() << std::endl;
        std::cout << "Data size: " << header->getSubchunk2Size() << std::endl;
    } else {
        throw std::runtime_error("Failed to read file header");
    }
}

void Parser::printAudioData() const {
    if (valid) {
        const auto& audioData = header->getAudioData();
        std::ofstream outFile("../audio_data.bin", std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Failed to open output file");
        }
        outFile.write(reinterpret_cast<const char*>(audioData.data()), audioData.size() * sizeof(int16_t));
        outFile.close();
        std::cout << "Audio data saved to audio_data.bin" << std::endl;
    } else {
        throw std::runtime_error("Failed to read audio data");
    }
}

void Parser::printOtherChunks() const {
    if (valid) {
        const auto& otherChunks = header->getOtherChunks();
        for (const auto& [key, value] : otherChunks) {
            std::ofstream outFile("../" + key + ".bin", std::ios::binary);
            if (!outFile) {
                throw std::runtime_error("Failed to open output file");
            }
            outFile.write(value.data(), value.size());
            outFile.close();
            std::cout << "Chunk " << key << " saved to " << key << ".bin" << std::endl;
        }
    } else {
        throw std::runtime_error("Failed to read other chunks");
    }
}

void Parser::printWaveform() const {
    constexpr int width = 80;
    constexpr int height = 20;
    constexpr int mid = height / 2;
    const auto audioData = header->getAudioData();

    std::vector waveform(height, std::string(width, ' '));

    for (size_t i = 0; i < width; ++i) {
        const size_t index = i * audioData.size() / width;
        const int sample = audioData[index];

        int y = mid + sample * mid / 32768;
        y = std::clamp(y, 0, height - 1);

        waveform[y][i] = '█';
    }

    for (const auto& line : waveform) {
        std::cout << line << std::endl;
    }
}
