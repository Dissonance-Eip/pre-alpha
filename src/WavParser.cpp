//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>

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

void Parser::printOtherChunks() const {
    if (valid) {
        const auto& otherChunks = header->getOtherChunks();
        for (const auto& [key, value] : otherChunks) {
            if (key == "LIST") {
                printListChunk(value);
            } else {
                printGenericChunk(key, value);
            }
        }
    } else {
        throw std::runtime_error("Failed to read other chunks");
    }
}

void Parser::printListChunk(const std::vector<char>& value) const {
    std::cout << "Chunk LIST data:" << std::endl;
    std::string title;
    std::string url;
    std::string date;
    std::string name;
    std::string description;
    std::string software;

    size_t i = 0;
    while (i < value.size()) {
        if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "INFO") {
            i += 8; // Skip "INFO" and size
            title = std::string(value.begin() + i, value.begin() + i + 4);
            i += 4;
        } else if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "IART") {
            i += 8; // Skip "IART" and size
            title = std::string(value.begin() + i, value.begin() + i + 4);
            i += 4;
        } else if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "ICMT") {
            i += 8; // Skip "ICMT" and size
            url = std::string(value.begin() + i, value.begin() + i + 44);
            i += 44;
        } else if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "ICRD") {
            i += 8; // Skip "ICRD" and size
            date = std::string(value.begin() + i, value.begin() + i + 9);
            i += 9;
        } else if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "INAM") {
            i += 8; // Skip "INAM" and size
            name = std::string(value.begin() + i, value.begin() + i + 44);
            i += 44;
        } else if (i + 4 <= value.size() && std::string(value.begin() + i, value.begin() + i + 4) == "ISFT") {
            i += 8; // Skip "ISFT" and size
            software = std::string(value.begin() + i, value.begin() + i + 14);
            i += 14;
        } else {
            ++i;
        }
    }

    std::cout << "Title: " << title << std::endl;
    std::cout << "URL: " << url << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Software: " << software << std::endl;
}

void Parser::printGenericChunk(const std::string& key, const std::vector<char>& value) const {
    std::cout << "Chunk " << key << " data:" << std::endl;
    for (size_t i = 0; i < value.size(); ++i) {
        char ch = value[i];
        std::cout << (std::isprint(ch) ? ch : '.');
        if ((i + 1) % 16 == 0 || i == value.size() - 1) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}