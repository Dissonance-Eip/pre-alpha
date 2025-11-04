//
// Created by noe on 16/03/2025.
//

#include <iostream>
#include <memory>
#include <stdexcept>
#include "WavGUI.hpp"
#include <conio.h>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <path_to_wav_file>");
        }

        if (const auto parser = std::make_shared<GUI>(argv[1]); parser->isValid()) {
            parser->printMetadata();
            parser->printOtherChunks();
            parser->printWaveform();

            std::cout << "Commands: L/l decrease low, L/L increase low, M/m decrease mid, M/M increase mid, H/h decrease high, H/H increase high" << std::endl;
            std::cout << "Press 'p' to play with current EQ. While playing: Space to pause/resume, Enter to send Ctrl+C (raise SIGINT). 'q' to quit." << std::endl;

            float low = 0.0f, mid = 0.0f, high = 0.0f;
            parser->setLowGainDb(low);
            parser->setMidGainDb(mid);
            parser->setHighGainDb(high);

            bool running = true;
            while (running) {
                if (_kbhit()) {
                    int ch = _getch();
                    if (ch == 'q' || ch == 'Q') {
                        running = false;
                        parser->stopPlayback();
                        break;
                    } else if (ch == 'p' || ch == 'P') {
                        parser->startPlayback();
                    } else if (ch == 'l') {
                        low -= 1.0f; parser->setLowGainDb(low); std::cout << "Low: " << low << " dB\n";
                    } else if (ch == 'L') {
                        low += 1.0f; parser->setLowGainDb(low); std::cout << "Low: " << low << " dB\n";
                    } else if (ch == 'm') {
                        mid -= 1.0f; parser->setMidGainDb(mid); std::cout << "Mid: " << mid << " dB\n";
                    } else if (ch == 'M') {
                        mid += 1.0f; parser->setMidGainDb(mid); std::cout << "Mid: " << mid << " dB\n";
                    } else if (ch == 'h') {
                        high -= 1.0f; parser->setHighGainDb(high); std::cout << "High: " << high << " dB\n";
                    } else if (ch == 'H') {
                        high += 1.0f; parser->setHighGainDb(high); std::cout << "High: " << high << " dB\n";
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        } else {
            throw std::runtime_error("WAV file invalid.");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}