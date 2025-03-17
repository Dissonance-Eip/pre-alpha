//
// Created by noe on 16/03/2025.
//
#include "WavParser.hpp"
#include <iostream>
#include <fstream>

Parser::Parser(const std::string& filename) : header{} {
    std::cout << "Opening file: " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        std::cout << "File opened successfully" << std::endl;
        file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
        if (file.gcount() == sizeof(WavHeader)) {
            valid = true;
        } else {
            valid = false;
        }
        valid = true;
    } else {
        valid = false;
    }
}

bool Parser::isValid() const {
    return valid;
}

void Parser::printMetadata() const {
    if (valid) {
        std::cout << "Chunk size: " << header.chunkSize << std::endl;
        std::cout << "Audio format: " << header.audioFormat << std::endl;
        std::cout << "Number of channels: " << header.numChannels << std::endl;
        std::cout << "Sample rate: " << header.sampleRate << std::endl;
        std::cout << "Byte rate: " << header.byteRate << std::endl;
        std::cout << "Block align: " << header.blockAlign << std::endl;
        std::cout << "Bits per sample: " << header.bitsPerSample << std::endl;
        std::cout << "Data size: " << header.subchunk2Size << std::endl;
    } else {
        std::cerr << "WAV file invalid.\n";
    }
}

void Parser::printAudioData() const {
    // print audio data information
}