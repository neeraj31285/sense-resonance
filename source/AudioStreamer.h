#pragma once

#include <vector>
#include <atomic>
#include <portaudio.h>

#include "AdacConfig.h"

//Analog to Digital audio converter
namespace adac {

    //Continious Listener & sampler.
    class AudioStreamer 
    {
        void *m_stream;
        bool m_isBufferReady;
        
        std::size_t m_sampleStartIndex;
        std::vector<float> m_audioStream;
        std::atomic<bool> m_isCpaturingSample;

        void initAudioStream();
        void terminateAudioStream();
        
        AudioStreamer();
        ~AudioStreamer();

        static int audioCallback(const void *pInputBuffer, void *pOutputBuffer,
                                unsigned long pFramesPerBuffer,
                                const PaStreamCallbackTimeInfo *pTimeInfo,
                                PaStreamCallbackFlags pStatusFlags,
                                void *pUserBuffer);

    public:

        AudioStreamer(AudioStreamer&) = delete;
        AudioStreamer(AudioStreamer&&) = delete;
        void operator=(AudioStreamer&) = delete;

        bool startStream();
        bool captureSample(std::vector<float>& pAudioSample);
	
        static AudioStreamer& instance() {
            static AudioStreamer instance;
            return instance;
        }
    };
}
