#pragma once

namespace console
{
    constexpr short SAMPLE_COUNT = 100;

    constexpr const char* RED =         "\033[38;2;204;0;0m";
    constexpr const char* CYAN =        "\033[1;96m";
    constexpr const char* TEAL =        "\033[1;38;2;0;128;128m";
    constexpr const char* RED_BG =      "\033[1;41m";
    constexpr const char* GREEN =       "\033[0;32m";
    constexpr const char* GREEN_BOLD =  "\033[1;32m";
    constexpr const char* YELLOW =      "\033[38;2;255;255;153m";
    constexpr const char* YELLOW_BOLD = "\033[1;33m";
    constexpr const char* BLUE =        "\033[38;2;0;191;255m";
    constexpr const char* BRIGHT_RED =  "\033[1;31m";
    constexpr const char* RESET =       "\033[0m";
    constexpr const char* MOVE_UP =     "\033[A";
    constexpr const char* CLEAR_LINE =  "\033[2K";
    
    constexpr const char* BLUE_ZSTR =   "(L0) Resonating   ";
    constexpr const char* GREEN_ZSTR =  "(L1) Subtle-Burst ";
    constexpr const char* YELLOW_ZSTR = "(L2) Mild-Burst   ";
    constexpr const char* RED_ZSTR =    "(L3) Audible-Burst";
    constexpr const char* DANGER_ZSTR = "(L4) Rattling";

    constexpr const char* ANOMALY_STR =         "anomaly";
    constexpr const char* ANOMALY_STATE =       "Anomaly State:";
    constexpr const char* ANOMLY_SAFE_STR =     "Stable (No-Risk)";
    constexpr const char* ANOMLY_GREEN_STR =    "Threshold exceeded. (Caution)";
    constexpr const char* ANOMLY_YELLOW_STR =   "Detecting resonance. (Critical)";
    constexpr const char* ANOMLY_DANGER_STR =   "Severe vibrations. (Fatal)";
    constexpr const char* ANOMALY_ALERT_STR =   "[ALERT: Glasses Rattling!!]";
    
    #define ANOMALY_ZONE_GREEN(_anomalyScore) (_anomalyScore > 0.85f && _anomalyScore < 0.97f)
    #define ANOMALY_ZONE_YELLOW(_anomalyScore) (_anomalyScore >= 0.97f && _anomalyScore < 0.9999f)
    #define ANOMALY_ZONE_DANGER(_anomalyScore) (_anomalyScore >= 0.9999f)

    #define PEAK_ZONE_BLUE(_peakConfidence) (_peakConfidence > 0.85f && _peakConfidence < 0.92f)
    #define PEAK_ZONE_GREEN(_peakConfidence) (_peakConfidence >= 0.92f && _peakConfidence < 0.96f)
    #define PEAK_ZONE_YELLOW(_peakConfidence) (_peakConfidence >= 0.96f && _peakConfidence < 0.99f)
    #define PEAK_ZONE_RED(_peakConfidence) (_peakConfidence >= 0.99f && _peakConfidence < 0.9999f)
    #define PEAK_ZONE_DANGER(_peakConfidence) (_peakConfidence >= 0.9999f)
}