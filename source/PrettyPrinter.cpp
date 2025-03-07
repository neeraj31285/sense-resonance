
#include "PrettyPrinter.h"
#include "AiConfig.h"
#include "PrintConstants.h"

#include <cmath>
#include <mutex>
#include <thread>
#include <chrono>
#include <iomanip>
#include <iostream>


namespace console 
{
    std::mutex g_consoleMutex;
    
    PrettyPrinter::PrettyPrinter()
    : m_peakConfidence(0.0f)
    , m_tickCounter(0)
    , m_peakSamples(SAMPLE_COUNT, 0.0)
    {
        startPulseTimer();
    }
    
    
    PrettyPrinter::~PrettyPrinter()
    {
    
    }


    std::vector<std::pair<std::string, float>> &PrettyPrinter::getStatsBuffer() {
        return m_statsBuffer;
    }


    void PrettyPrinter::update()
    {
        if (m_statsBuffer[0].second > m_peakConfidence) {
            m_peakConfidence = m_statsBuffer[0].second;
        }
    }


    float PrettyPrinter::computeRMS(const std::vector<float>& pSamples)
    {
        if (!pSamples.empty()) 
        {
            float sumOfSquares = 0.0f;
            for (float sample : pSamples) {
                sumOfSquares += (sample * sample);
            }
            return std::sqrt(sumOfSquares / pSamples.size());
        }
        return 0.0f;
    }


    bool PrettyPrinter::startStatsTimer()
    {
        std::thread([this]()
        {
            while (true) {    
                //critical section
                {
                    std::lock_guard<std::mutex> lock(g_consoleMutex);
                    updateStats();
                }
                static unsigned short counter = 0;
                const auto& index = (counter++ % SAMPLE_COUNT);
                m_peakSamples[index] = m_peakConfidence;        
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }).detach();
        return true;
    }
    
    
    void PrettyPrinter::startPulseTimer()
    {
        std::thread([this]()
        {
            while (true) {
                if(m_statsBuffer.size() < ai::LABEL_COUNT) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }
                //critical section
                {
                    std::lock_guard<std::mutex> lock(g_consoleMutex);
                    updatePulse();
                    m_peakConfidence = 0.0;
                    static bool _= startStatsTimer();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }).detach();
    }
    
    
    void PrettyPrinter::updatePulse()
    {
        std::cout   << "\n" << CLEAR_LINE <<" "
                    << m_statsBuffer[ai::INDEX_0].first <<" (" << std::fixed << std::setprecision(5)
                    << m_statsBuffer[ai::INDEX_0].second << ")\t"
                    << m_statsBuffer[ai::INDEX_1].first <<" ("
                    << m_statsBuffer[ai::INDEX_1].second << ")\t"
                    << ANOMALY_STR << " (" << m_peakConfidence << ")"
                    << "\n-----------------------------------------------------------------          \n";

        const float& anomalyScore = computeRMS(m_peakSamples);
        if ANOMALY_ZONE_GREEN(anomalyScore)
        {
            std::cout   << " " << CYAN
                        << ANOMALY_STATE << RESET
                        << " " << GREEN_BOLD
                        << ANOMLY_GREEN_STR << RESET;
        }
        else if ANOMALY_ZONE_YELLOW(anomalyScore)
        {
            std::cout   << " " << CYAN
                        << ANOMALY_STATE << RESET
                        <<" " << YELLOW_BOLD
                        << ANOMLY_YELLOW_STR << RESET;
        }
        else if ANOMALY_ZONE_DANGER(anomalyScore)
        {
            std::cout   <<" " << CYAN
                        << ANOMALY_STATE << RESET
                        << " " << BRIGHT_RED
                        <<  ANOMLY_DANGER_STR << RESET
                        <<" \t" << RED_BG
                        << ANOMALY_ALERT_STR << RESET;
        }
        else
        {
            std::cout   << " " << TEAL
                        << ANOMALY_STATE << CYAN
                        << " " << ANOMLY_SAFE_STR << RESET;
        }
        std::cout   << MOVE_UP << MOVE_UP << std::flush;
    }
    
    
    void PrettyPrinter::updateStats()
    {
        std::cout   << "\r" << "\033[11C"
                    << std::fixed << std::setprecision(5)
                    << m_statsBuffer[ai::INDEX_0].second << "\033[16C"
                    << m_statsBuffer[ai::INDEX_1].second << "\033[15C";
    
        if PEAK_ZONE_BLUE(m_peakConfidence)
        {
            std::cout   << BLUE
                        << "(" << m_peakConfidence
                        << ")\t" << BLUE_ZSTR << RESET;
        }
        else if PEAK_ZONE_GREEN(m_peakConfidence)
        {
            std::cout   << GREEN
                        << "(" << m_peakConfidence
                        << ")\t" << GREEN_ZSTR << RESET;
        }
        else if PEAK_ZONE_YELLOW(m_peakConfidence)
        {
            std::cout   << YELLOW
                        << "(" << m_peakConfidence
                        << ")\t" << YELLOW_ZSTR << RESET;
        }
        else if PEAK_ZONE_RED(m_peakConfidence)
        {
            std::cout   << RED
                        << "(" << m_peakConfidence
                        << ")\t" << RED_ZSTR << RESET;
        }
        else if PEAK_ZONE_DANGER(m_peakConfidence)
        {
            std::cout   << BRIGHT_RED
                        << "(" << m_peakConfidence
                        << ")\t" << DANGER_ZSTR << RESET;
        }
        else {
            std::cout << "(" << m_peakConfidence << ") ";
        }
        std::cout << std::flush;
    }
}