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

    extern "C" __TWExport void FileDialogPanel(std::string externalFilter, FileDialogSelectInfo &selectInfo);

    extern "C" __TWExport void MainMenuBar();
} // namespace TwinkleGraphics
#endif
