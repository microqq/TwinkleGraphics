#include "twApplication.h"
#include "twEditorWindow.h"

using namespace TwinkleGraphics;

int main(int, char **) {
  EditorWindow editor(2560, 1440);
  auto &app = ApplicationInstance();
  app.SetMainFrame(&editor);

  app.Run();
  return 0;
}
