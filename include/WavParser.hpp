//
// Created by noe on 16/03/2025.
//

#ifndef WAVPARSER_H
#define WAVPARSER_H

#include <memory>
#include <string>
#include "WavHeader.hpp"

class Parser {
public:
    Parser(const std::string& filename);
    bool isValid() const;
    void printMetadata() const;
    void printAudioData() const;

private:
    std::shared_ptr<WavHeader> header;
    bool valid = false;
};

#endif // WAVPARSER_H