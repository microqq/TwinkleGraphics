
#include "ImGuiFileDialog.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "twComUtil.h"
#include "twCustomFont_1.h"
#include "twEditorMenu.h"

namespace TwinkleGraphics {

bool showDemoWindow = false;

EditorMenu::EditorMenu(Widget *parent) : Widget(parent), _modelFileDialog() {
  _modelFileDialogKey = RandomString();

  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, ".obj",
  //                               ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, ".OBJ",
  //                               ImVec4(0.0f, 1.0f, 1.0f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, "fbx",
  //                               ImVec4(0.0f, 1.0f, 0.0f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, ".FBX",
  //                               ImVec4(0.0f, 0.0f, 1.0f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, ".3ds",
  //                               ImVec4(1.0f, 0.0f, 1.0f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByExtention, ".3DS",
  //                               ImVec4(0.0f, 1.0f, 0.5f, 0.9f));
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByContainedInFullName, ".gltf",
  //                               ImVec4(0.9f, 0.2f, 0.0f, 0.9f));

  // define style for all directories
  _modelFileDialog.SetFileStyle(IGFD_FileStyleByTypeDir, "",
                                ImVec4(0.9f, 0.9f, 0.9f, 0.9f),
                                ICON_IGFD_FOLDER);
  // // can be for a specific directory
  // _modelFileDialog.SetFileStyle(IGFD_FileStyleByTypeDir, ".git",
  //                               ImVec4(0.5f, 1.0f, 0.9f, 0.9f),
  //                               ICON_IGFD_FOLDER);
  // define style for all files
  _modelFileDialog.SetFileStyle(IGFD_FileStyleByTypeFile, "",
                                ImVec4(0.9f, 0.9f, 0.0f, 0.9f), ICON_IGFD_CUBE);
}

EditorMenu::~EditorMenu() {}

void EditorMenu::OnGui() {
  if (ImGui::BeginMainMenuBar()) {
    DrawFileMenu();
    DrawEditMenu();
    DrawSceneNodeMenu();
    DrawHelpMenu();

    ImGui::EndMainMenuBar();
  }

  DisplayModelFileDialog();
}

void EditorMenu::OpenModelFileDialog() {
  _modelFileDialog.OpenDialog(_modelFileDialogKey,
                              ICON_IGFD_FOLDER_OPEN " Choose File",
                              ".obj,.OBJ,.fbx,.FBX,.3ds,.3DS,.gltf", ".");
}
void EditorMenu::DisplayModelFileDialog() {
  // display
  if (_modelFileDialog.Display(_modelFileDialogKey, ImGuiWindowFlags_NoCollapse,
                               ImVec2(800, 600), ImVec2(1600, 1200))) {
    // action if OK
    if (_modelFileDialog.IsOk()) {
      std::string filePathName = _modelFileDialog.GetFilePathName();
      std::string filePath = _modelFileDialog.GetCurrentPath();
      // action
      std::string fileName = _modelFileDialog.GetCurrentFileName();

      Console::LogInfo(filePathName, "\n");
      Console::LogInfo(filePath, "\n");
      Console::LogInfo(fileName, "\n");
    }

    // close
    _modelFileDialog.Close();
  }
}

void EditorMenu::DrawFileMenu() {
  if (ImGui::BeginMenu("File")) {
    ImGui::Separator();
    ImGui::EndMenu();
  }
}

void EditorMenu::DrawEditMenu() {
  if (ImGui::BeginMenu("Edit")) {
    ImGui::Separator();
    ImGui::EndMenu();
  }
}

void EditorMenu::DrawSceneNodeMenu() {
  if (ImGui::BeginMenu("SceneNode")) {
    ImGui::Separator();

    if (ImGui::MenuItemEx("Empty", ICON_IGFD__446)) {
    }

    if (ImGui::MenuItemEx("Load Mesh...", ICON_IGFD_FILE)) {
      OpenModelFileDialog();
    }

    DrawGeometryMenu();
    ImGui::Separator();

    if (ImGui::MenuItemEx("Camera", ICON_IGFD_CAMERA)) {
    }
    if (ImGui::MenuItemEx("Light", ICON_IGFD_SUN)) {
    }

    ImGui::EndMenu();
  }
}

void EditorMenu::DrawGeometryMenu() {
  if (ImGui::BeginMenuEx("Geometry", ICON_IGFD_CUBES, true)) {
    if (ImGui::MenuItem("Triangle")) {
    }
    if (ImGui::MenuItem("Quad")) {
    }
    if (ImGui::MenuItem("Cube")) {
    }
    if (ImGui::MenuItem("Plane")) {
    }
    if (ImGui::BeginMenu("Sphere")) {
      if (ImGui::MenuItem("Ico Sphere")) {
      }
      if (ImGui::MenuItem("Cubic Sphere")) {
      }
      if (ImGui::MenuItem("Standard Sphere")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Bezier Curve")) {
    }
    if (ImGui::MenuItem("Bezier Surface")) {
    }
    if (ImGui::MenuItem("NURBS Curve")) {
    }
    if (ImGui::MenuItem("NURBS Surface")) {
    }

    ImGui::EndMenu();
  }
}

void EditorMenu::DrawLayoutMenu() {}

void EditorMenu::DrawHelpMenu() {
  if (ImGui::BeginMenu("Help")) {
    ImGui::MenuItem("Show ImGui Demo", "", &showDemoWindow);
    ImGui::EndMenu();
  }

  if (showDemoWindow) {
    ImGui::ShowDemoWindow(&showDemoWindow);
  }
}

} // namespace TwinkleGraphics
