cmake_minimum_required(VERSION 3.10)
project(dissonance.pre-alpha)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/WavGui.cpp
    src/Equalizer.cpp
    src/AudioPlayer.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(dissonance.pre-alpha ${SOURCES})

# Link Windows multimedia library when building on Windows to allow PlaySound
if (WIN32)
    target_link_libraries(dissonance.pre-alpha PRIVATE winmm)
endif()
