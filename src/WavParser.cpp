//
// Created by noe on 16/03/2025.
//

#include "WavParser.hpp"


Parser::Parser(const std::string& filename) : header(std::make_shared<WavHeader>()), filename(filename) {
    std::cout << colors[0]<< "Opening file: " << colors[3] <<filename << colors[4] << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::cout << colors[3] <<"File opened successfully" << colors[4] << std::endl;
    header->readFromFile(file);
    valid = true;
}

void Parser::printMetadata() const {
    if (valid) {
        std::cout << colors[0] << "Chunk size: " << colors[1] << header->getChunkSize() << colors[4] << std::endl;
        std::cout << colors[0] << "Audio format: " << colors[1] << header->getAudioFormat() << colors[4] << std::endl;
        std::cout << colors[0] << "Number of channels: " << colors[1] << header->getNumChannels() << colors[4] << std::endl;
        std::cout << colors[0] << "Sample rate: " << colors[1] << header->getSampleRate() << colors[4] << std::endl;
        std::cout << colors[0] << "Byte rate: " << colors[1] << header->getByteRate() << colors[4] << std::endl;
        std::cout << colors[0] << "Block align: " << colors[1] << header->getBlockAlign() << colors[4] << std::endl;
        std::cout << colors[0] << "Bits per sample: " << colors[1] << header->getBitsPerSample() << colors[4] << std::endl;
        std::cout << colors[0] << "Data size: " << colors[1] << header->getSubchunk2Size() << colors[4] << std::endl;
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
                std::cout << colors[3] << ch << colors[4];
            } else {
                std::cout << ch;
            }
        }
        std::cout << '\n';
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
    std::cout << std::endl << colors[3] << "MetaData:" << colors[4] << std::endl;
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

    std::cout << colors[0] << "Title: "<< colors[1] << title << colors[4] << std::endl;
    std::cout << colors[0] << "URL: " << colors[1] << url << colors[4] << std::endl;
    std::cout << colors[0] << "Date: " << colors[1] << date << colors[4] << std::endl;
    std::cout << colors[0] << "Name: " << colors[1] << name << colors[4] << std::endl;
    std::cout << colors[0] << "Description: " << colors[1] << description << colors[4] << std::endl;
    std::cout << colors[0] << "Software: " << colors[1] << software << colors[4] << std::endl;
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