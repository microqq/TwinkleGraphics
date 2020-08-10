#ifndef TW_CONSOLELOG_H
#define TW_CONSOLELOG_H

#include <iostream>

#include "twCommon.h"

namespace TwinkleGraphics
{
    namespace Console
    {
#if defined _WIN32
        // https://docs.microsoft.com/en-us/windows/console/using-the-high-level-input-and-output-functions
        enum class Color
        {
            GRAY = 7,
            BLUE = 9,
            GREEN = 10,
            CYAN = 11,
            RED = 12,
            MAGENTA = 13,
            YELLOW = 14,
            WHITE = 15
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

        namespace Internal
        {
            extern void SetConsoleColor(Color &c);
            extern void ResetConsoleColor();

            // https://www.codeproject.com/articles/16431/add-color-to-your-std-cout
            template <class _Elem, class _Traits>
            std::basic_ostream<_Elem, _Traits> &
            operator<<(std::basic_ostream<_Elem, _Traits> &os, Color &c)
            {
                SetConsoleColor(c);
                return os;
            }

            template <class CharT>
            void Log(Color color, CharT t)
            {
                std::cout << color << t;
                ResetConsoleColor();
            }

            template <class CharT, class... Args>
            void Log(Color color, CharT t, Args... args)
            {
                Log(color, t);
                Log(color, args...);
            }

        } // namespace Internal

        template <Color C, class... Args>
        void LogWithColor(Args... args)
        {
#ifdef _DEBUG
            Internal::Log(C, args...);
#endif
        }

        template <class... Args>
        void LogGTestInfo(Args... args)
        {
#ifdef _DEBUG
            Internal::Log(Color::GREEN, "[ INFO     ] ", args...);
#endif
        }

        template <class... Args>
        void LogInfo(Args... args)
        {
#ifdef _DEBUG
            Internal::Log(Color::WHITE, args...);
#endif
        }

        template <class... Args>
        void LogWarning(Args... args)
        {
#ifdef _DEBUG
            Internal::Log(Color::YELLOW, args...);
#endif
        }

        template <class... Args>
        void LogError(Args... args)
        {
#ifdef _DEBUG
            Internal::Log(Color::RED, args...);
#endif
        }

    } // namespace Console
} // namespace TwinkleGraphics

#endif