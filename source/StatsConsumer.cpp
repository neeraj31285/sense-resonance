#include "StatsConsumer.h"
#include "AiConfig.h"

#include <mutex>
#include <thread>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <iostream>

constexpr short SAMPLE_COUNT = 5;
constexpr char* RED = "\033[1;31m";
constexpr char* CYAN = "\033[1;96m";
constexpr char* TEAL = "\033[38;2;0;128;128m";
constexpr char* RED_BG = "\033[1;41m";
constexpr char* GREEN = "\033[1;32m";
constexpr char* YELLOW = "\033[1;33m";
constexpr char* MAGENTA = "\033[1;35m";
constexpr char* BRIGHT_RED = "\033[38;2;204;0;0m";
constexpr char* RESET = "\033[0m";
constexpr char* MOVE_UP = "\033[A";
constexpr char* CLEAR_LINE = "\033[2K";

std::mutex g_consoleMutex;

StatsConsumer::StatsConsumer()
: m_peakConfidence(0.0f)
, m_tickCounter(0)
, m_peakSamples(SAMPLE_COUNT, 0.0)
{
    startPulseTimer();
}


StatsConsumer::~StatsConsumer()
{
    
}


std::vector<std::pair<std::string, float>>& StatsConsumer::getContainer()
{
    return m_stats;
}


bool StatsConsumer::startStatsTimer()
{
    std::thread([this]()
    {
        int counter = 0;
        while (true) 
        {
            if (m_stats[0].second > m_peakConfidence) {
                m_peakConfidence = m_stats[0].second;
            } 
            //critical section
            {
                std::lock_guard<std::mutex> lock(g_consoleMutex);
                updateStats();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }).detach();
    return true;
}


void StatsConsumer::startPulseTimer()
{
    std::thread([this]()
    {
        while (true)
        {
            std::cout << std::flush;
            if(m_stats.size() <= 1) {
                continue;
            } 
            //critical section
            {
                std::lock_guard<std::mutex> lock(g_consoleMutex);                
                std::cout   << "\n" << CLEAR_LINE <<" " 
                            << m_stats[ai::INDEX_0].first <<" (" << std::fixed << std::setprecision(5) 
                            << m_stats[ai::INDEX_0].second << ")\t" 
                            << m_stats[ai::INDEX_1].first <<" (" 
                            << m_stats[ai::INDEX_1].second << ")\t"
                            << "anomaly " << "(" << m_peakConfidence << ")"
                            << "\n-----------------------------------------------------------------          \n";

                updatePulse();
                std::cout << MOVE_UP << MOVE_UP << std::flush;
                static bool _= startStatsTimer();
                m_peakConfidence = 0.0;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}


void StatsConsumer::updatePulse()
{
    static std::size_t counter = 0;
    const short& lastIndex = (counter++ % SAMPLE_COUNT);
    m_peakSamples[lastIndex] = m_peakConfidence;
    const float& anomalyScore = std::accumulate(m_peakSamples.begin(), m_peakSamples.end(), 0.0f) / SAMPLE_COUNT;

    if(anomalyScore > 0.9f && anomalyScore < 0.997f) 
    {
        std::cout   << " " << CYAN 
                    << "Anomaly State:" << RESET 
                    << " " << GREEN 
                    << "Threshold exceeded. (Caution)" << RESET;
    }
    else if(anomalyScore >= 0.997f && anomalyScore < 0.999f) 
    {
        std::cout   << " " << CYAN 
                    << "Anomaly State:" << RESET 
                    <<" " << YELLOW 
                    << "Detecting resonance. (Critical)" << RESET;
    }
    else if(anomalyScore >= 0.999f) 
    {
        std::cout   <<" " << CYAN 
                    << "Anomaly State:" << RESET 
                    << " " << BRIGHT_RED 
                    << "Severe vibrations. (Fatal)" << RESET 
                    <<" \t" << RED_BG 
                    << "[ALERT: Glasses rattling!!]" << RESET;
    }
    else 
    {
        std::cout   << " " << TEAL 
                    << "Anomaly State:" << CYAN 
                    << " Stable (No-Risk)" << RESET;
    }
}


void StatsConsumer::updateStats()
{
    std::cout   << "\r" << "\033[11C"
                << std::fixed << std::setprecision(5) 
                << m_stats[ai::INDEX_0].second << "\033[16C"
                << m_stats[ai::INDEX_1].second << "\033[15C";
    
    if(m_peakConfidence > 0.85f && m_peakConfidence < 0.92f) 
    {
        std::cout   << GREEN 
                    << "(" << m_peakConfidence
                    << ")\t(L1) Resonating   " << RESET;
    }
    else if(m_peakConfidence > 0.92f && m_peakConfidence < 0.96f) 
    {
        std::cout   << YELLOW 
                    << "(" << m_peakConfidence
                    << ")\t(L2) Subtle-Burst " << RESET;
    }
    else if(m_peakConfidence >= 0.96f && m_peakConfidence < 0.99f) 
    {
        std::cout   << MAGENTA 
                    << "(" << m_peakConfidence 
                    << ")\t(L3) Mild-Burst   " << RESET;
    }
    else if(m_peakConfidence >= 0.99f && m_peakConfidence < 0.9999f) 
    {
        std::cout   << RED
                    << "(" << m_peakConfidence
                    << ")\t(L4) Audible-Burst" << RESET;
    }
    else if(m_peakConfidence >= 0.9999f) 
    {
        std::cout   << BRIGHT_RED
                    << "(" << m_peakConfidence
                    << ")\t(L5) Rattling" << RESET
                    << "     ";
    }
    else {
        std::cout << "(" << m_peakConfidence    << ") ";
    }
    std::cout << std::flush;
}