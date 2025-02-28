#pragma once

#include <vector>
#include <string>
#include <iostream>

class StatsConsumer
{
    static std::size_t m_tickCounter;
    static std::vector<float> m_peakSamples;
    
    static bool startTimer();
    
    static void updateSummary(const float& pConfidence);

    static bool updatePeakScore(const float& pConfidence);

public:

    static void update(std::vector<std::pair<std::string, float>>& pConfidence);
};
