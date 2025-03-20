//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"


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
        std::cout << colors[0] << "Chunk size: " << colors[1] << header->getChunkSize() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Audio format: " << colors[1] << header->getAudioFormat() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Number of channels: " << colors[1] << header->getNumChannels() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Sample rate: " << colors[1] << header->getSampleRate() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Byte rate: " << colors[1] << header->getByteRate() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Block align: " << colors[1] << header->getBlockAlign() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Bits per sample: " << colors[1] << header->getBitsPerSample() << "\033[0m" << std::endl;
        std::cout << colors[0] << "Data size: " << colors[1] << header->getSubchunk2Size() << "\033[0m" << std::endl;
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
    constexpr int defaultWidth = 200;
    constexpr int defaultHeight = 30;
    constexpr int width = defaultWidth;
    constexpr int height = defaultHeight;
    constexpr int mid = height / 2;
    const auto audioData = header->getAudioData();

    std::vector waveform(height, std::string(width, ' '));

    const int16_t maxSample = *std::max_element(audioData.begin(), audioData.end());
    const int16_t minSample = *std::min_element(audioData.begin(), audioData.end());
    const int range = maxSample - minSample;

    int prevY = mid;

    for (size_t i = 0; i < width; ++i) {
        const size_t index = i * audioData.size() / width;
        const int sample = audioData[index];

        int y = mid + (sample - minSample) * (height - 1) / range - mid;
        y = std::clamp(y, 0, height - 1);

        if (y != prevY) {
            for (int j = std::min(y, prevY); j <= std::max(y, prevY); ++j) {
                waveform[j][i] = '|';
            }
        } else {
            waveform[y][i] = '|';
        }
        prevY = y;
    }

    for (const auto& line : waveform) {
        for (const auto& ch : line) {
            if (ch == '|') {
                std::cout << "\033[32m" << ch << "\033[0m";
            } else {
                std::cout << ch;
            }
        }
        std::cout << '\n';
    }
}