//
// Created by noe on 16/03/2025.
//

#include <iostream>
#include <memory>
#include "WavParser.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_wav_file>\n";
        return 1;
    }

    std::shared_ptr<Parser> parser = std::make_shared<Parser>(argv[1]);
    if (parser->isValid()) {
        parser->printMetadata();
        parser->printAudioData();
    } else {
        std::cerr << "WAV file invalid.\n";
    }
    return 0;
}
