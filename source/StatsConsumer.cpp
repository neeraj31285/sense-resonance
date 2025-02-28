#include "StatsConsumer.h"

#include <thread>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>

constexpr short SAMPLE_COUNT = 3;

std::size_t StatsConsumer::m_tickCounter = 0;
std::vector<float> StatsConsumer::m_peakSamples(SAMPLE_COUNT, 0.0);

bool StatsConsumer::startTimer() 
{
    std::thread([]() 
    {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            m_tickCounter++;
        }
    }).detach();
    return true;
}


void StatsConsumer::updateSummary(const float& pConfidence)
{
    static std::size_t counter = 0;
    const short& lastIndex = (counter++ % SAMPLE_COUNT);
    m_peakSamples[lastIndex] = pConfidence;
    const float& anomalyScore = std::accumulate(m_peakSamples.begin(), m_peakSamples.end(), 0.0f) / SAMPLE_COUNT;

    if(anomalyScore > 0.9f && anomalyScore < 0.997f) {
        std::cout <<" \033[1;96mAnomaly State:\033[0m \033[1;32mThreshold exceeded. (Low-risk)\033[0m";     // Green (Safe)
    }
    else if(anomalyScore >= 0.997f && anomalyScore < 0.999f) {
        std::cout <<" \033[1;96mAnomaly State:\033[0m \033[1;33mDetecting resonance. (High-risk)\033[0m";   // Red (Danger)
    }
    else if(anomalyScore >= 0.999f) {
        std::cout <<" \033[1;96mAnomaly State:\033[0m \033[1;35mSevere vibrations. \033[1;31m(CRITICAL)\033[0m \t\033[1;41m[ALERT: Glasses rattling!!]\033[0m"; // Red Background (Critical)
    }
    else {
        std::cout <<" \033[1;96mAnomaly State:\033[0m Stable (No-Risk)";
    }
}


bool StatsConsumer::updatePeakScore(const float& pConfidence)
{
    if(pConfidence > 0.85f && pConfidence < 0.92f) {
        std::cout << "\033[1;32m(" << pConfidence << ")\t[Level 0] Resonating.\033[0m";    // Green (Safe)
    }
    else if(pConfidence > 0.92f && pConfidence < 0.96f) {
        std::cout << "\033[1;33m(" << pConfidence << ")\t[Level 1] Subtle burst.\033[0m";    // Yellow (Warning)
    }
    else if(pConfidence >= 0.96f && pConfidence < 0.99f) {
        std::cout << "\033[1;35m(" << pConfidence << ")\t[Level 2] Mild burst.\033[0m";    // Magenta (Caution)
    }
    else if(pConfidence >= 0.99f && pConfidence < 0.9999f) {
        std::cout << "\033[1;31m(" << pConfidence << ")\t[Level 3] Audible burst.\033[0m";    // Red (Danger)
    }
    else if(pConfidence >= 0.9999f) {
        std::cout << "\033[1;41m(" << pConfidence << ")\t[Level 4] Rattling.\033[0m"; // Red Background (Critical)
    }
    else {
        std::cout << "(" << pConfidence << ")" << std::flush;
	    return false;
    }
    return true;
}


void StatsConsumer::update(std::vector<std::pair<std::string, float>>& pConfidence) 
{
    static bool _= startTimer();
    static bool isColored = false;
    static float peakConfidence = 0.0f;
    static int lastTick = m_tickCounter;
    
    if (pConfidence[0].second > peakConfidence) {
        peakConfidence = pConfidence[0].second;
    }

    std::cout   << "\r"; // Move cursor a line up and to start of line.
    std::cout   << " " <<pConfidence[0].first <<" (" << std::fixed << std::setprecision(5) << pConfidence[0].second << ")\t"
                << pConfidence[1].first <<" (" << pConfidence[1].second << ")\t"
                << "anomaly ";
    
    if(lastTick != m_tickCounter) 
    {
          std::cout   << "\n\33[2K\r" << std::flush;
          std::cout   << " " << pConfidence[0].first <<" (" << std::fixed << std::setprecision(5) << pConfidence[0].second << ")\t"
                      << pConfidence[1].first <<" (" << pConfidence[1].second << ")\t"
                      << "anomaly ";

        isColored = updatePeakScore(peakConfidence);
        std::cout << "\n-----------------------------------------------------------------          " << std::endl;
        updateSummary(peakConfidence);
        std::cout << "\033[A\033[A";
        lastTick = m_tickCounter;
        peakConfidence = 0.0;
    }
    else if (!isColored) {
        std::cout << "(" << peakConfidence << ")" << std::flush;
    }
}
