#ifndef TW_CONSOLELOG_H
#define TW_CONSOLELOG_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>
#include "twCommon.h"

namespace TwinkleGraphics
{
    namespace Console
    {
        // https://docs.microsoft.com/en-us/windows/console/using-the-high-level-input-and-output-functions
#ifdef _WIN32
        enum class Color
        {
            GRAY = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
            BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
            MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
            YELLOW = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
            WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
        };
#elif defined __linux__
        // http://www.cplusplus.com/forum/unices/36461/
        enum class Color
        {
            GRAY = 30,
            RED = 31,
            GREEN = 32,
            YELLOW = 33,
            BLUE = 34,
            MAGENTA = 35,
            CYAN = 36,
            WHITE = 37
        };
#endif

        template <class CharT>
        void Log(CharT &&t)
        {
            std::cout << t;
        }

        template <class CharT, class... Args>
        void Log(CharT &&t, Args &&... args)
        {
            Log(std::forward<CharT>(t));
            Log(std::forward<Args>(args)...);
        }

        extern "C" void SetStdoutColor(Color color);

        template <class... Args>
        void LogInfo(Args &&... args)
        {
            SetStdoutColor(Color::WHITE);
            Log(std::forward<Args>(args)...);
        }

        template <class... Args>
        void LogWarning(Args &&... args)
        {
            SetStdoutColor(Color::YELLOW);
            Log(std::forward<Args>(args)...);
        }

        template <class... Args>
        void LogError(Args &&... args)
        {
            SetStdoutColor(Color::RED);
            Log(std::forward<Args>(args)...);
        }

    } // namespace Console
} // namespace TwinkleGraphics

#endif