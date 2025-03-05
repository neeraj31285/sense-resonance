 
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>

#include "AudioStreamer.h"
#include "AudioDspUtils.h"

namespace adac
{
    AudioStreamer::AudioStreamer()
    : m_stream(nullptr)
    , m_isBufferReady(false)
    , m_sampleStartIndex(0)
    , m_isCpaturingSample(false)
    {
        m_audioStream.reserve(adac::BUFFER_SIZE);
        initAudioStream();
    }


    AudioStreamer::~AudioStreamer()
    {
        terminateAudioStream();
    }


    void AudioStreamer::terminateAudioStream()
    {
        Pa_StopStream(m_stream);
        Pa_CloseStream(m_stream);
        Pa_Terminate();
        std::cout << "AudioStreamer: terminated." << std::endl;
    }


    bool AudioStreamer::startStream()
    {
        std::cout << "AudioStreamer: starting stream...";
        const auto& err = Pa_StartStream(m_stream);
        if (err != paNoError) {
            std::cout << "AudioStreamer: error! " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
	    std::cout << "done" << std::endl;
        return true;
    }


    bool AudioStreamer::captureSample(std::vector<float>& pAudioSample)
    {
        if(m_isBufferReady)
        {
            m_isCpaturingSample = true;
            {
                for(int i = 0; i < adac::SAMPLE_SIZE; i++)
                {
                    const auto& index = ((m_sampleStartIndex + i) % adac::BUFFER_SIZE);
                    pAudioSample.push_back(m_audioStream.at(index));
                }
            }
            m_isCpaturingSample = false;
            return true;
        }
	    else 
        {
            static auto lastTStamp = std::chrono::high_resolution_clock::now();
            const auto& curTStamp = std::chrono::high_resolution_clock::now();
            const auto& elapsedT = std::chrono::duration_cast<std::chrono::milliseconds>(curTStamp - lastTStamp);
            if(elapsedT.count() % 1000 == 0) {
                std::cout << "\r";
                std::cout << "AudioStreamer: buffering... (" << (elapsedT.count() / 1000) << "s)        ";
            }
            std::cout << std::flush;
	    }
        return false;
    }


    int AudioStreamer::audioCallback(const void *pInputBuffer, void *pOutputBuffer,
                                     unsigned long pFramesPerBuffer,
                                     const PaStreamCallbackTimeInfo *pTimeInfo,
                                     PaStreamCallbackFlags pStatusFlags,
                                     void *pUserBuffer)
    {
        static unsigned int counter = 0;
        const float *inputBuffer = static_cast<const float*> (pInputBuffer);
        auto *userAudioStream = static_cast<std::vector<float>*> (pUserBuffer);

        for(int i = 0; i < pFramesPerBuffer; i++)
        {
            const auto& index = (counter++ % adac::BUFFER_SIZE);
            if(userAudioStream->size() < adac::BUFFER_SIZE)
            {
                userAudioStream->push_back(inputBuffer[i]);
                if(userAudioStream->size() == adac::BUFFER_SIZE) {
                    std::cout << "\nAudioStreamer: audio buffer ready." << std::endl;
                    instance().m_isBufferReady = true;
                }
            }
            else {
                int count = 0;
                while (instance().m_isCpaturingSample)
                {
                    if(count > 1) {
                        std::cout << "AudioStreamer: streaming paused while capturing sample. Time elapsed: " << count++ << "ms" << std::endl;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                userAudioStream->at(index) = inputBuffer[i];
                instance().m_sampleStartIndex = ((adac::BUFFER_SIZE + (index - adac::SAMPLE_SIZE)) % adac::BUFFER_SIZE);
            }
        }

        return paContinue;
    }


    void AudioStreamer::initAudioStream()
    {
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            std::cout << "AudioStreamer: error! " << Pa_GetErrorText(err) << "." << std::endl;
            exit(1);
        }
        else {
            std::cout << "AudioStreamer: portaudio initialized." << std::endl;
        }

        int deviceId = Pa_GetDefaultInputDevice();
        if(adac::DEVICE_ID == -1)
        {
            const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(deviceId);
            if (!deviceInfo) {
                std::cout << "No default input device found!\n";
                exit(1);
            }
            else {
                std::cout << "AudioStreamer: using audio device: "<< deviceInfo->name << " Id(" << deviceId <<"), SampleRate("<< deviceInfo->defaultSampleRate << "Hz)" << std::endl;
            }
        }
        else {
            std::cout << "AudioStreamer: using explicit audio device, Id: " << adac::DEVICE_ID << std::endl;
        }

        PaStreamParameters inputParameters;
	    inputParameters.device = (adac::DEVICE_ID == -1 ? deviceId : adac::DEVICE_ID);
        inputParameters.channelCount = adac::NUM_CHANNELS;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

        std::cout << "AudioStreamer: setting up stream...";
        err = Pa_OpenStream(&m_stream, &inputParameters, nullptr, adac::SAMPLE_RATE, adac::FRAMES_PER_BUFFER,
                            paClipOff, audioCallback, &m_audioStream);

        if (err != paNoError) {
            std::cout << "AudioStreamer: error! " << Pa_GetErrorText(err) << "." << std::endl;
            terminateAudioStream();
            exit(1);
        }
        else {
            std::cout << "done" << std::endl;
        }
    }
}
