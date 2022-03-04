#include "twApplication.h"
#include "twEditorWindow.h"

using namespace TwinkleGraphics;

int main(int, char **) {
  EditorWindow editor(1024, 768);
  auto &app = ApplicationInstance();
  app.SetMainFrame(&editor);

  app.Run();
  return 0;
}