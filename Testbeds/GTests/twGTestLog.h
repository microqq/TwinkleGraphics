
// https://stackoverflow.com/questions/16491675/how-to-send-custom-message-in-google-c-testing-framework
// It not worked.....!!!!!!!!!!! So
// I changed gtest.h/gtest.cpp
// gtest.cpp: remove GTestColor defination, add into gtest.h
// gtest.h: like this
// namespace testing
//{
//  namespace internal
//  {
//      enum class GTestColor { kDefault, kRed, kGreen, kYellow };
//      GTEST_API_ GTEST_ATTRIBUTE_PRINTF_(2, 3) void ColoredPrintf(GTestColor
//      color,
//                                                            const char* fmt,
//                                                            ...);
//  }
//}

#ifndef TW_GTESTLOG_H
#define TW_GTESTLOG_H

#include <gtest/gtest.h>
#include <sstream>


namespace testing {
namespace internal {
extern void ColoredPrintf(testing::internal::GTestColor color, const char *fmt,
                          ...);
} // namespace internal
} // namespace testing
#define PRINTF(...)                                                            \
  do {                                                                         \
    testing::internal::ColoredPrintf(testing::internal::GTestColor::kGreen,    \
                                     "[          ] ");                         \
    testing::internal::ColoredPrintf(testing::internal::GTestColor::kYellow,   \
                                     __VA_ARGS__);                             \
  } while (0)

// C++ stream interface
class TestCout : public std::stringstream {
public:
  ~TestCout() { PRINTF("%s", str().c_str()); }
};

#define TEST_COUT TestCout()

#endif