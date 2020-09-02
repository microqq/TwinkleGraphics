

#ifndef TW_SCENEMANAGEMENT_H
#define TW_SCENEMANAGEMENT_H

#include "twCommon.h"
#include "twCamera.h"

#define MAX_SCENE_CAMERA_COUNT 128

namespace TwinkleGraphics
{

class __TWExport Scene : public Object
{
public:
    typedef std::shared_ptr<Scene> Ptr;

    Scene();
    virtual ~Scene();

    virtual void Init();
    virtual void Update(float32 delta_time);
    virtual void Render();

    void SetMainCamera(Camera::Ptr cam)
    { 
        _maincamera = cam;
    }

    void AddCamera(Camera::Ptr cam)
    {
        if(cam == nullptr)
            return;
        _cameralists[++_validCameraCount] = cam;
    }
    void RemoveCamera(int32 index)
    {
        if(index < 0 || index >= MAX_SCENE_CAMERA_COUNT)
        {
            return;
        }
        _cameralists[index] = nullptr;
    }
    Camera::Ptr GetCamera(int32 index)
    {
        if(index < 0 || index >= MAX_SCENE_CAMERA_COUNT)
        {
            return nullptr;
        }
        return _cameralists[index];
    }

    void SortCamera() {}

protected:
    Camera::Ptr _maincamera;
    Camera::Ptr _cameralists[MAX_SCENE_CAMERA_COUNT];
    ISceneNode _sceneroot;

    int32 _validCameraCount;
};  
} // namespace TwinkleGraphics

#endif