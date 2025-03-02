#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <atomic>

namespace console 
{
    class PrettyPrinter
    {
        float m_peakConfidence;
        std::size_t m_tickCounter;
        std::vector<float> m_peakSamples;
        std::vector<std::pair<std::string, float>> m_stats;
    
        void updateStats();
        void updatePulse();
    
        bool startStatsTimer();
        void startPulseTimer();
    
        PrettyPrinter();
        ~PrettyPrinter();
    
    public:
    
        PrettyPrinter(PrettyPrinter&) = delete;
        PrettyPrinter(PrettyPrinter&&) = delete;
        void operator=(PrettyPrinter&) = delete;
    
        static PrettyPrinter& instance() {
            static PrettyPrinter instance;
            return instance;
        }
        
        std::vector<std::pair<std::string, float>>& getOutBuffer();
    };
}