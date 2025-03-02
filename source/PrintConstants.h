#pragma once

namespace console
{
    constexpr short SAMPLE_COUNT = 30;

    constexpr char* RED = "\033[38;2;204;0;0m";
    constexpr char* CYAN = "\033[1;96m";
    constexpr char* TEAL = "\033[1;38;2;0;128;128m";
    constexpr char* RED_BG = "\033[1;41m";
    constexpr char* GREEN = "\033[0;32m";
    constexpr char* GREEN_BOLD = "\033[1;32m";
    constexpr char* YELLOW = "\033[38;2;255;255;153m";
    constexpr char* YELLOW_BOLD = "\033[1;33m";
    constexpr char* BLUE = "\033[38;2;0;191;255m";
    constexpr char* BRIGHT_RED = "\033[1;31m";
    constexpr char* RESET = "\033[0m";
    constexpr char* MOVE_UP = "\033[A";
    constexpr char* CLEAR_LINE = "\033[2K";
}