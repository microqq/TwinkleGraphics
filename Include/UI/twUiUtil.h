#ifndef TW_UIUTIL_H
#define TW_UIUTIL_H

#include "twCommon.h"

namespace TwinkleGraphics
{
    struct FileDialogSelectInfo
    {
        std::string filePathName;
        std::string filePath;
        std::string fileName;
        bool selectChanged = false;
    };
    extern FileDialogSelectInfo SceneNodeFileSelectInfo;

// #ifdef __cplusplus
//     extern "C"
//     {
// #endif
        __TWExport void FileDialogPanel(std::string externalFilter, FileDialogSelectInfo &selectInfo);
        __TWExport void MainMenuBar();
// #ifdef __cplusplus
//     }
// #endif

} // namespace TwinkleGraphics
#endif
