

#ifndef TW_EDITOR_MENU_H
#define TW_EDITOR_MENU_H

#include "ImGuiFileDialog.h"

#include "twWidget.h"

namespace TwinkleGraphics {

class __TWCOMExport EditorMenu : public Widget {
public:
  explicit EditorMenu(Widget *parent = nullptr);
  virtual ~EditorMenu();

  virtual void OnGui() override;

private:
  void OpenModelFileDialog();
  void DisplayModelFileDialog();

  void DrawFileMenu();
  void DrawEditMenu();
  void DrawSceneNodeMenu();
  void DrawGeometryMenu();
  void DrawLayoutMenu();
  void DrawHelpMenu();

private:
  ImGuiFileDialog _modelFileDialog;
  std::string _modelFileDialogKey;
};
} // namespace TwinkleGraphics

#endif
