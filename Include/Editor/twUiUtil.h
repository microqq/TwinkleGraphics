#ifndef TW_UIUTIL_H
#define TW_UIUTIL_H

#include "twCommon.h"

namespace TwinkleGraphics {
struct FileDialogSelectInfo {
  std::string filePathName;
  std::string filePath;
  std::string fileName;
  bool selectChanged = false;
};

#ifdef __cplusplus
extern "C" {
#endif
__TWAPI void FileDialogPanel(std::string externalFilter,
                                   FileDialogSelectInfo &selectInfo);
__TWAPI void MainMenuBar();
#ifdef __cplusplus
}
#endif

} // namespace TwinkleGraphics
#endif
