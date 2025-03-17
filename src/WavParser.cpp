//
// Created by noe on 16/03/2025.
//
#include "WavParser.hpp"
#include <iostream>
#include <fstream>

Parser::Parser(const std::string& filename) {
    std::cout << "Opening file: " << filename << std::endl;
    if (std::ifstream file(filename, std::ios::binary); file) {
        std::cout << "File opened successfully" << std::endl;
        // parse file header then data
        valid = true;
    } else {
        valid = false;
    }
}

bool Parser::isValid() const {
    return valid;
}

void Parser::printMetadata() const {
    // print metadata information
}

void Parser::printAudioData() const {
    // print audio data information
}