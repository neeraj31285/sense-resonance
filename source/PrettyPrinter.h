#pragma once

#include <vector>
#include <string>

namespace console 
{
    class PrettyPrinter
    {
        float m_peakConfidence;
        std::size_t m_tickCounter;
        std::vector<float> m_peakSamples;
        std::vector<std::pair<std::string, float>> m_statsBuffer;
    
        PrettyPrinter();
        ~PrettyPrinter();

        void updateStats();
        void updatePulse();
    
        bool startStatsTimer();
        void startPulseTimer();
        float computeRMS(const std::vector<float>& pSamples);

    public:
    
        PrettyPrinter(PrettyPrinter&) = delete;
        PrettyPrinter(PrettyPrinter&&) = delete;
        void operator=(PrettyPrinter&) = delete;
    
        static PrettyPrinter& instance() {
            static PrettyPrinter instance;
            return instance;
        }
        
        void update();
        
        std::vector<std::pair<std::string, float>>& getStatsBuffer();
    };
}