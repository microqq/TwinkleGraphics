
#ifndef TW_FIRSTTRIANGLE_H
#define TW_FIRSTTRIANGLE_H

#include "twCommon.h"

#include "twPlugin.h"

namespace TwinkleGraphics
{
class FirstTriangle : public GLPlugin
{
public:
    FirstTriangle(std::string& name);
    ~FirstTriangle();

    virtual void Install() override;
    virtual void UnInstall() override;

private:

};
} // namespace TwinkleGraphics

#endif