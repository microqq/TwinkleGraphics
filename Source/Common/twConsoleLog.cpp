#if defined _WIN32
#ifdef __cplusplus
extern "C"
{
#endif
#include <windows.h>
#ifdef __cplusplus
}
#endif
#endif

#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    namespace Console
    {

        namespace Internal
        {

            void SetConsoleColor(Color &c)
            {
#ifdef _WIN32
                HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hStdout, (WORD)c);
#elif defined __linux__

#endif
            }

            void ResetConsoleColor()
            {
                Color c = Color::WHITE;
                SetConsoleColor(c);
            }
        } // namespace Internal
    }     // namespace Console

} // namespace TwinkleGraphics
