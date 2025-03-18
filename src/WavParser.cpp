//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"
#include <iostream>
#include <fstream>

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
        std::ofstream outFile("../audio_data.txt");
        if (!outFile) {
            throw std::runtime_error("Failed to open output file");
        }
        for (const auto& sample : audioData) {
            outFile << sample << "\n";
        }
        outFile.close();
        std::cout << "Audio data saved to audio_data.txt" << std::endl;
    } else {
        throw std::runtime_error("Failed to read audio data");
    }
}
