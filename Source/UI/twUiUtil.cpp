#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "twUiUtil.h"

namespace TwinkleGraphics
{
    FileDialogSelectInfo SceneNodeFileSelectInfo;

    void FileDialogPanel(std::string externalFilter, FileDialogSelectInfo &selectInfo)
    {
        // https://github.com/aiekick/ImGuiFileDialog
        // open Dialog Simple
        std::string display = selectInfo.filePathName;
        if (display.empty())
        {
            display = "\"Please select file.\"";
        }
        ImGui::Text(display.c_str());
        ImGui::SameLine();
        if (ImGui::Button("Open File"))
        {
            igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", externalFilter.c_str(), ".");
        }

        // display
        if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
        {
            // action if OK
            if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
            {
                selectInfo.filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
                selectInfo.filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                selectInfo.fileName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
                // action

                if (selectInfo.filePathName != display)
                {
                    selectInfo.selectChanged = true;
                }
            }
            // close
            igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
        }
    }

    void MainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("SceneNode"))
            {
                if (ImGui::MenuItem("SceneNode Empty"))
                {
                }
                if (ImGui::MenuItem("SceneNode From File..."))
                {
                    const char* extensionFilter = ".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gsm,.GSM,.lwo,.LWO,.meshdata,.MESHDATA,.geo,.GEO";
                    igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", extensionFilter, ".");
                }

                // display
                if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
                {
                    // action if OK
                    if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
                    {
                        SceneNodeFileSelectInfo.filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
                        SceneNodeFileSelectInfo.filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                        SceneNodeFileSelectInfo.fileName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
                        // action
                    }
                    // close
                    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
                }

                if (ImGui::MenuItem("3D Object"))
                {
                }
                if (ImGui::MenuItem("2D Object"))
                {
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Camera"))
                {
                }
                if (ImGui::MenuItem("Light"))
                {
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
} // namespace TwinkleGraphics