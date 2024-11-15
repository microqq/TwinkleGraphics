#include "twUiUtil.h"
#include "ImGuiFileDialog.h"
#include "imgui.h"

#include "twImGuiContextManager.h"

namespace TwinkleGraphics {
void FileDialogPanel(std::string externalFilter,
                     FileDialogSelectInfo &selectInfo) {
  // https://github.com/aiekick/ImGuiFileDialog
  // open Dialog Simple
  std::string display = selectInfo.filePathName;
  if (display.empty()) {
    display = "\"Please select file.\"";
  }

  ImGuiContextManager &imguiCtxMgr = ImGuiContextMgrInstance();
  imguiCtxMgr.SetCurrentContext();

  ImGui::Text(display.c_str());
  ImGui::SameLine();
  if (ImGui::Button("Open File")) {
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                            externalFilter.c_str(), ".");
  }

  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      selectInfo.filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      selectInfo.filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      selectInfo.fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
      // action

      if (selectInfo.filePathName != display) {
        selectInfo.selectChanged = true;
      }
    }
    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void MainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("SceneNode")) {
      if (ImGui::MenuItem("SceneNode Empty")) {
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
    if (ImGui::BeginMenu("Other")) {
      if (ImGui::MenuItem("Sample Scenes")) {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
} // namespace TwinkleGraphics