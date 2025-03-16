//
// Created by noe on 16/03/2025.
//

#ifndef WAVPARSER_H
#define WAVPARSER_H

#include <string>

class Parser {
public:
    Parser(const std::string& filename);
    bool isValid() const;
    void printMetadata() const;
    void printAudioData() const;

private:
    bool valid = false;
    // add other members as needed
};

#endif // WAVPARSER_H