//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"
#include <iostream>
#include <fstream>

Parser::Parser(const std::string& filename) : header(std::make_shared<WavHeader>()) {
    std::cout << "Opening file: " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        std::cout << "File opened successfully" << std::endl;
        file.read(reinterpret_cast<char*>(header.get()), sizeof(WavHeader));
        if (file.gcount() == sizeof(WavHeader)) {
            valid = true;
        } else {
            valid = false;
        }
    } else {
        valid = false;
    }
}

bool Parser::isValid() const {
    return valid;
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
        std::cerr << "WAV file invalid.\n";
    }
}

void Parser::printAudioData() const {
    // print audio data information
}