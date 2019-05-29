
#ifndef TW_FIRSTTRIANGLE_H
#define TW_FIRSTTRIANGLE_H

#include "twCommon.h"

#include "twPlugin.h"

namespace TwinkleGraphics
{
class TriangleView;

class FirstTriangle : public GLPlugin
{
public:
    FirstTriangle(std::string& name);
    ~FirstTriangle();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    void InitTriangleView();

private:
    TriangleView* _view;
};

class TriangleView : public View
{
public:
    TriangleView(Viewport& viewport)
        : View(viewport)
    {}
    virtual ~TriangleView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void RenderImplement() override;
    
    friend class FirstTriangle;

private:
    uint32* _vaos;
    uint32* _vbos;
    uint32* _ebos;
};

} // namespace TwinkleGraphics

#endif