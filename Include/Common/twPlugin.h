
#ifndef TW_PLUGIN_H
#define TW_PLUGIN_H

#include <iostream>
#include <string>

#include "twConsoleLog.h"

namespace TwinkleGraphics {
class Plugin {
public:
  Plugin(std::string &name) : _name(name) {}
  virtual ~Plugin() {}

  std::string &GetName() { return _name; }

  virtual void Install() {
    Console::LogInfo("------------------------------Plugin Install:", _name,
                     "------------------------------\n");
  }
  virtual void UnInstall() {
    Console::LogInfo("------------------------------Plugin UnInstall:", _name,
                     "------------------------------\n");
  }

protected:
  std::string _name;
};

} // namespace TwinkleGraphics

#endif