
#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "twEditorMenu.h"

namespace TwinkleGraphics
{
    EditorMenu::EditorMenu(Widget *parent)
        : Widget(parent)
    {
    }

    EditorMenu::~EditorMenu()
    {
    }

    void EditorMenu::OnGui()
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
                    const char *extensionFilter = ".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gsm,.GSM,.lwo,.LWO,.meshdata,.MESHDATA,.geo,.GEO";
                    igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", extensionFilter, ".");
                }

                // display
                if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
                {
                    // action if OK
                    if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
                    {
                        // SceneNodeFileSelectInfo.filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
                        // SceneNodeFileSelectInfo.filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
                        // SceneNodeFileSelectInfo.fileName = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
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
            if (ImGui::BeginMenu("Sample Scenes"))
            {
                // if (ImGui::MenuItem("Sample Scenes"))
                // {
                // }
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
