#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <atomic>

class StatsConsumer
{
    float m_peakConfidence;
    std::size_t m_tickCounter;
    std::vector<float> m_peakSamples;
    std::vector<std::pair<std::string, float>> m_stats;

    void updateStats();
    void updatePulse();

    bool startStatsTimer();
    void startPulseTimer();

    StatsConsumer();
    ~StatsConsumer();

public:

    StatsConsumer(StatsConsumer&) = delete;
    StatsConsumer(StatsConsumer&&) = delete;
    void operator=(StatsConsumer&) = delete;

    static StatsConsumer& instance() {
        static StatsConsumer instance;
        return instance;
    }

    std::vector<std::pair<std::string, float>>& getContainer();
};
