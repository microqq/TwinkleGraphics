#ifndef TW_IMGUI_CONTEXT_MANAGER_H
#define TW_IMGUI_CONTEXT_MANAGER_H

#include "imgui.h"
#include "twCommon.h"
#include "twSingleton.h"

namespace TwinkleGraphics {
class __TWAPI ImGuiContextManager : public INonCopyable {
public:
  explicit ImGuiContextManager() { _pCurrentContext = ImGui::CreateContext(); }
  ~ImGuiContextManager() { _pCurrentContext = nullptr; }

  // 提供获取上下文的方法
  ImGuiContext *GetCurrentContext() { return _pCurrentContext; }

  void SetCurrentContext() {
    IM_ASSERT(_pCurrentContext != nullptr);
    ImGui::SetCurrentContext(_pCurrentContext);
  }

  void SetAllocatorFunctions() {
    ImGui::SetAllocatorFunctions(*_pAllocFunc, *_pFreeFunc, nullptr);
  }
  
  void GetAllocatorFunctions() {
    ImGui::GetAllocatorFunctions(_pAllocFunc, _pFreeFunc, nullptr);
  }

private:
  ImGuiContext *_pCurrentContext{nullptr};
  ImGuiMemAllocFunc *_pAllocFunc{nullptr};
  ImGuiMemFreeFunc *_pFreeFunc{nullptr};
  friend class Singleton<ImGuiContextManager>;
};

#ifdef __cplusplus
extern "C" {
#endif

__TWAPI ImGuiContextManager &ImGuiContextMgrInstance();

#ifdef __cplusplus
}
#endif

} // namespace TwinkleGraphics

#endif
