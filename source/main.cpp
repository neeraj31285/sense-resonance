#include <portaudio.h>
#include <iostream>
#include <vector>

#include "Classifier.h"
#include "AudioStreamer.h"
#include "AudioDspUtils.h"
#include "StatsConsumer.h"

int main() 
{
    std::vector<float> sample;

    sample.reserve(adac::SAMPLE_SIZE);
    if(!adac::AudioStreamer::instance().startStream()) {
        std::cout << "failed to start stream.." << std::endl;
        return -1;
    }
    
    while (true)
    {
        if(adac::AudioStreamer::instance().captureSample(sample))
        {
            adac::AudioDspUtils::scaleFloatToPCM(sample);
            auto& aiResult = StatsConsumer::instance().getContainer();
            ai::Classifier::runInference(sample, aiResult);
        }
        sample.clear();
    }
    return 0;
}