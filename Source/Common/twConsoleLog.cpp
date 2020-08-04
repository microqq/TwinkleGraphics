

#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    namespace Console
    {
        void SetStdoutColor(Color color)
        {
#if defined _WIN32 || _WIN64
            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdout, (WORD)(color));
#elif defined __linux__

#endif
        }

    } // namespace Console

} // namespace TwinkleGraphics
