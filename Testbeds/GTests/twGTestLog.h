
// https://stackoverflow.com/questions/16491675/how-to-send-custom-message-in-google-c-testing-framework
// It not worked.....!!!!!!!!!!!

#ifndef TW_GTESTLOG_H
#define TW_GTESTLOG_H

#include <sstream>

namespace testing
{
    namespace internal
    {
        enum GTestColor
        {
            COLOR_DEFAULT,
            COLOR_RED,
            COLOR_GREEN,
            COLOR_YELLOW
        };

        extern "C" void ColoredPrintf(GTestColor color, const char *fmt, ...);
    } // namespace internal
} // namespace testing

namespace TwinkleGraphics
{
    #define PRINTF(...)                                                                        \
        do                                                                                     \
        {                                                                                      \
            testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); \
            testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__);    \
        } while (0)

    // C++ stream interface
    class TestCout : public std::stringstream
    {
    public:
        ~TestCout()
        {
            PRINTF("%s", str().c_str());
        }
    };

    #define TEST_COUT TestCout()
    
} // namespace TwinkleGraphics


#endif