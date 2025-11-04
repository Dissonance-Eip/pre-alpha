//
// Created by noe on 16/03/2025.
//

#include "WavGUI.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    #include <conio.h>
    #include <csignal>
#endif

#include <thread>
#include <chrono>


GUI::GUI(const std::string& filename) : data(std::make_shared<Parser>()), filename(filename) {
    std::cout << colors[0]<< "Opening file: " << colors[3] <<filename << colors[4] << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::cout << colors[3] <<"File opened successfully" << colors[4] << std::endl;
    data->readFromFile(file);
    // Reinitialize EQ to match file sample rate and channels
    eq = Equalizer(data->getSampleRate(), data->getNumChannels());
    valid = true;
}

void GUI::printMetadata() const {
    if (valid) {
        std::cout << colors[0] << "Chunk size: " << colors[1] << data->getChunkSize() << colors[4] << std::endl;
        std::cout << colors[0] << "Audio format: " << colors[1] << data->getAudioFormat() << colors[4] << std::endl;
        std::cout << colors[0] << "Number of channels: " << colors[1] << data->getNumChannels() << colors[4] << std::endl;
        std::cout << colors[0] << "Sample rate: " << colors[1] << data->getSampleRate() << colors[4] << std::endl;
        std::cout << colors[0] << "Byte rate: " << colors[1] << data->getByteRate() << colors[4] << std::endl;
        std::cout << colors[0] << "Block align: " << colors[1] << data->getBlockAlign() << colors[4] << std::endl;
        std::cout << colors[0] << "Bits per sample: " << colors[1] << data->getBitsPerSample() << colors[4] << std::endl;
        std::cout << colors[0] << "Data size: " << colors[1] << data->getSubchunk2Size() << colors[4] << std::endl;
    } else {
        throw std::runtime_error("Failed to read file data");
    }
}

void GUI::printAudioData() const {
    if (valid) {
        const auto& audioData = data->getAudioData();
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

void GUI::printWaveform() const {
    constexpr int defaultWidth = 200;
    constexpr int defaultHeight = 30;
    constexpr int width = defaultWidth;
    constexpr int height = defaultHeight;
    constexpr int mid = height / 2;
    const auto audioData = data->getAudioData();

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

void GUI::printOtherChunks() const {
    if (valid) {
        const auto& otherChunks = data->getOtherChunks();
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

#include <iomanip>
#include <sstream>

void GUI::printListChunk(const std::vector<char>& value) const {
    std::cout << std::endl << colors[3] << "MetaData:" << colors[4] << std::endl;
    std::unordered_map<std::string, std::string*> chunkMap = {
        {"INAM", &title},
        {"IART", &name},
        {"ICMT", &description},
        {"ICRD", &date},
        {"ISFT", &software},
        {"IGNR", &genre},
        {"ICOP", &copyright}
    };

    size_t i = 0;
    while (i < value.size()) {
        if (std::string chunkId(value.begin() + i, value.begin() + i + 4); chunkMap.find(chunkId) != chunkMap.end()) {
            i += 4; // Skip chunk ID
            const uint32_t chunkSize = *reinterpret_cast<const uint32_t*>(&value[i]);
            i += 4; // Skip chunk size
            const auto& field = chunkMap[chunkId];
            *field = std::string(value.begin() + i, value.begin() + i + chunkSize);
            i += chunkSize;

            field->erase(std::find_if(field->rbegin(), field->rend(), [](const unsigned char ch) {
                return !std::isspace(ch) && ch != '\0';
            }).base(), field->end());
        } else {
            ++i;
        }
    }

    if (!date.empty() && date.size() == 8) {
        std::ostringstream formattedDate;
        formattedDate << date.substr(0, 4) << "-" << date.substr(4, 2) << "-" << date.substr(6, 2);
        date = formattedDate.str();
    }

    std::cout << colors[0] << "Title: " << colors[1] << title << colors[4] << std::endl;
    std::cout << colors[0] << "Date: " << colors[1] << date << colors[4] << std::endl;
    std::cout << colors[0] << "Name: " << colors[1] << name << colors[4] << std::endl;
    std::cout << colors[0] << "Description: " << colors[1] << description << colors[4] << std::endl;
    std::cout << colors[0] << "Software: " << colors[1] << software << colors[4] << std::endl;
    std::cout << colors[0] << "Genre: " << colors[1] << genre << colors[4] << std::endl;
    std::cout << colors[0] << "Copyright: " << colors[1] << copyright << colors[4] << std::endl;
}

void GUI::printGenericChunk(const std::string& key, const std::vector<char>& value) {
    std::cout << "Chunk " << key << " data:" << std::endl;
    for (size_t i = 0; i < value.size(); ++i) {
        const char ch = value[i];
        std::cout << (std::isprint(ch) ? ch : '.');
        if ((i + 1) % 16 == 0 || i == value.size() - 1) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

// EQ control
void GUI::setLowGainDb(float db) { eq.setLowGainDb(db); }
void GUI::setMidGainDb(float db) { eq.setMidGainDb(db); }
void GUI::setHighGainDb(float db) { eq.setHighGainDb(db); }

// Playback control: apply EQ and play via AudioPlayer. Handles Space to pause/resume and Enter to raise SIGINT.
bool GUI::startPlayback() {
#ifdef _WIN32
    if (!valid) return false;
    const auto& audio = data->getAudioData();
    const unsigned sampleRate = data->getSampleRate();
    const unsigned channels = data->getNumChannels();

    std::vector<int16_t> processed;
    eq.process(audio, processed);

    // Start playback using alias "mywav" so we can poll MCI status
    if (!player.playFromBuffer(processed, sampleRate, channels, "mywav")) {
        std::cerr << "Failed to start playback via AudioPlayer." << std::endl;
        return false;
    }

    std::cout << colors[1] << "Playing... (Space = pause/resume, Enter = send Ctrl+C)" << colors[4] << std::endl;

    bool paused = false;
    char statusBuf[128] = {0};

    while (true) {
        // Query MCI playback status
        if (mciSendStringA("status mywav mode", statusBuf, sizeof(statusBuf), nullptr) != 0) {
            break;
        }
        std::string status(statusBuf);
        while (!status.empty() && std::isspace(static_cast<unsigned char>(status.back()))) status.pop_back();

        if (status == "stopped" || status == "not ready" || status == "closed") {
            break;
        }

        if (_kbhit()) {
            int ch = _getch();
            if (ch == ' ') {
                if (paused) {
                    player.resume();
                    paused = false;
                    std::cout << colors[3] << "Resumed" << colors[4] << std::endl;
                } else {
                    player.pause();
                    paused = true;
                    std::cout << colors[2] << "Paused" << colors[4] << std::endl;
                }
            } else if (ch == 13) {
                std::cout << "Enter pressed: raising SIGINT (Ctrl+C)" << std::endl;
                player.stop();
                std::raise(SIGINT);
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    player.stop();
    return true;
#else
    (void)audio; (void)processed;
    std::cout << "Playback not implemented on this platform." << std::endl;
    return false;
#endif
}

void GUI::pausePlayback() { player.pause(); }
void GUI::resumePlayback() { player.resume(); }
void GUI::stopPlayback() { player.stop(); }
