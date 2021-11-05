
#include "ImGuiFileDialog.h"
#include "imgui.h"


#include "twEditorMenu.h"
#include "twUiUtil.h"

namespace TwinkleGraphics {
FileDialogSelectInfo modelFileSelectInfo;

EditorMenu::EditorMenu(Widget *parent) : Widget(parent) {}

EditorMenu::~EditorMenu() {}

void EditorMenu::OnGui() {
  // display model select panel
  std::string display = modelFileSelectInfo.filePathName;
  if (display.empty()) {
    display = "\"Please select file.\"";
  }

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::Separator();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      ImGui::Separator();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("SceneNode")) {
      ImGui::Separator();

      if (ImGui::MenuItem("SceneNode Empty")) {
      }

      if (ImGui::MenuItem("SceneNode From File...")) {
        const char *extensionFilter = ".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gltf";
        igfd::ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseModelDlgKey", "Choose File", extensionFilter, ".");
      }

      if (ImGui::MenuItem("3D Object")) {
      }
      if (ImGui::MenuItem("2D Object")) {
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Camera")) {
      }
      if (ImGui::MenuItem("Light")) {
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Sample Scenes")) {
      ImGui::Separator();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      ImGui::Separator();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseModelDlgKey")) {
    // action if OK
    if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {
      modelFileSelectInfo.filePathName =
          igfd::ImGuiFileDialog::Instance()->GetFilepathName();
      modelFileSelectInfo.filePath =
          igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
      modelFileSelectInfo.fileName =
          igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();
      // action

      if (modelFileSelectInfo.filePathName != display) {
        modelFileSelectInfo.selectChanged = true;
      }
    }
    // close
    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseModelDlgKey");
  }
}
} // namespace TwinkleGraphics
