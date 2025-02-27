#pragma once

//Analog to Digital audio converter
namespace adac
{
    constexpr short DEVICE_ID = -1;     //set -1 to use default.
    constexpr unsigned short NUM_CHANNELS = 1;    // Mono audio
    constexpr unsigned int SAMPLE_RATE = 44100;
	constexpr unsigned int SAMPLE_UNITS = 1;
    constexpr unsigned int SAMPLE_SIZE = (SAMPLE_UNITS * SAMPLE_RATE);
    constexpr unsigned int FRAMES_PER_BUFFER = 512;   // Number of frames per buffer
    constexpr unsigned short BUFFERED_SAMPLE_COUNT = 3;
    constexpr unsigned int BUFFER_SIZE = (BUFFERED_SAMPLE_COUNT * SAMPLE_SIZE);
}
