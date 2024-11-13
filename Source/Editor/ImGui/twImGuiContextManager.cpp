#include "twImGuiContextManager.h"

namespace TwinkleGraphics {

ImGuiContextManager &ImGuiContextMgrInstance() {
  return Singleton<ImGuiContextManager>::Instance();
}
} // namespace TwinkleGraphics
