//
// Created by noe on 16/03/2025.
//

#include <iostream>
#include "WavParser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_wav_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::cout << "Opening file: " << filename << std::endl;
    Parser wav(filename);
    if (wav.isValid()) {
        wav.printMetadata();
        wav.printAudioData();
    } else {
        std::cerr << "Failed to open WAV file." << std::endl;
    }
    return 0;
}