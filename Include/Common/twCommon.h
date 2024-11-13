

#ifndef TW_COMMON_H
#define TW_COMMON_H

#include <string>
#include <vector>

#include "twObject.h"
#include "twSingleton.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#ifndef MAX_VIEWPORT_COUNT
#define MAX_VIEWPORT_COUNT 8
#endif

#ifndef SAFE_DEL
#define SAFE_DEL(ptr)                                                          \
  if ((ptr) != nullptr) {                                                      \
    delete (ptr);                                                              \
    (ptr) = nullptr;                                                           \
  }
#endif

#ifndef SAFE_DEL_ARR
#define SAFE_DEL_ARR(ptr)                                                      \
  if ((ptr) != nullptr) {                                                      \
    delete[](ptr);                                                             \
    (ptr) = nullptr;                                                           \
  }
#endif

/**
 * @brief Refer to ogre plugin system. (https://ogrecave.github.io/ogre/)
 *
 */
#if defined(__linux__) || defined(__APPLE__)
// #ifdef EXPORT_COMDLL
#define __TWAPI __attribute__((__visibility__("default")))
// #endif

// #ifdef EXPORT_PLUGIN
#define __TWPLUGINAPI __attribute__((__visibility__("default")))
// #endif

#elif defined(_WIN32)

#ifdef EXPORT_COMDLL
#define __TWAPI __declspec(dllexport)
#else
#define __TWAPI __declspec(dllimport)
#endif

#ifdef EXPORT_PLUGIN
#define __TWPLUGINAPI __declspec(dllexport)
#else
#define __TWPLUGINAPI __declspec(dllimport)
#endif

#endif

namespace TwinkleGraphics {
using namespace glm;

class Plugin;
class PluginManager;
using INSTALL_PLUGIN_FUNC = Plugin *(*)(PluginManager *);
using UNINSTALL_PLUGIN_FUNC =void (*)(PluginManager *);
using IMGUI_FUNC = void (*)(void);

using RENDER_ROUTINE = void (*)(void);

using Rect = ivec4;
using RGBA = vec4;

struct RenderResourceHandle {
  union {
    uint64 hash = 0;
    struct {
      uint32 id : 32;
      uint32 type : 32;
    };
  };
};

class IUpdatable {
public:
  virtual void Update(float deltaTime = 0.0f) = 0;

protected:
  IUpdatable() {}
  virtual ~IUpdatable() {}
};

class INonCopyable {
private:
  INonCopyable(const INonCopyable &) = delete;
  INonCopyable(INonCopyable &&) = delete;
  INonCopyable &operator=(const INonCopyable &) = delete;
  INonCopyable &operator=(INonCopyable &&) = delete;

protected:
  INonCopyable() {}
  virtual ~INonCopyable() {}
};

class IDestroyable {
public:
  virtual void Destroy() = 0;
};

enum Intersection {
  OUTSIDE,
  INSIDE,
  INTERSECTING,
  INTERSECT = INSIDE | INTERSECTING
};

__TWAPI extern std::vector<std::string> GLExtensions;
__TWAPI extern bool CheckSupportExtension(std::string exention);

} // namespace TwinkleGraphics

#endif