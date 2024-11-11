### 1、ImGui context invalid

问题：twUIlib中创建了imgui context，twUIlib作为dll输出，这个时候exe执行文件twEditor想要调用这个context的时候会报错，提示imgui context无效。

解决办法：这个问题找了蛮长时间，最后发现可能是跨dll使用imgui context会导致报错，或者需要将imgui context设置为全局变量，这样就不会报错了？
[StackOverflow上遇到与我差不多的问题](https://stackoverflow.com/questions/75785506/invalid-imguicontext-when-it-should-be-valid)

“ImGui uses globals, which are not shared across DLL boundaries. The imgui.h and imgui.cpp contain solid answers to this problem. ”

[Sharing a global/static variable between a process and DLL](https://stackoverflow.com/questions/4911994/sharing-a-global-static-variable-between-a-process-and-dll)

[//---- Define attributes of all API symbols declarations, e.g. for DLL under Windows
// Using Dear ImGui via a shared library is not recommended, because of function call overhead and because we don't guarantee backward nor forward ABI compatibility.
// - Windows DLL users: heaps and globals are not shared across DLL boundaries! You will need to call SetCurrentContext() + SetAllocatorFunctions()
//   for each static/DLL boundary you are calling from. Read "Context and Memory Allocators" section of imgui.cpp for more details.](https://skia.googlesource.com/external/github.com/ocornut/imgui/+/refs/heads/master/imconfig.h)

(1) 一种解决问题的办法是：将twUIlib编译为静态库，这样，twEditor静态链接twEditor，这样twEditor可以直接获取ImGui context，而不用跨dll调用。

(2) 第二种办法是：twUIlib仍然编译为动态库，但是在twUIlib中输出创建的imgui context，以保证twEditor可以获取twUIlib中创建的imgui context。

(3) 还有一种办法是：因为imgui允许创建多个context，那么是否可以在察觉context invalid的时候，即时创建imgui context呢？ 试验了这种方法：创建context后，调用ImGui::SetCurrentContext(*)，仍然会报错，所以这种方法不可行？

(4) 暂时说结果吧，如果我要让twUIlib保持为dll时，那么便不能在该动态库内调用imgui context，否则会报错，所以，我选择将twUIlib调用imgui context的相关文件移出到twEditor中。

(5) 先说结果，第(4)条的猜测是错误的，正确定义一个类ImGuiContextManager，让其成为一个单例类，是可以做到在不同dll中共享同一个imgui context的：
```C++
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
```

需要注意的是，在不同的dll中，每个imgui domain都需要调用SetCurrentContext函数，以保证正确设置单例保存的上下文。

#### 2、twShading imgui button点击失效问题

问题：twShading执行时的几个插件按钮点击失效

解决办法：问题查了一段时间，最后发现是glfw窗口设置了输入事件回调函数，这样会导致imgui input event被截断？有些链接提供的办法说是要在imgui初始化之前先设置glfw的输入事件回到函数，不确定这样的说法是否正确，尝试过设置io.wantCaptureMouse等参数，发现并没有作用，不知是否设代码编写问题？

确认了下，在imgui初始化之前设置glfw窗口的输入事件回调函数，是可以避免imgui控件无法响应输入事件的。