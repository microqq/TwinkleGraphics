

#ifndef TW_SCENE_H
#define TW_SCENE_H

#include "twCommon.h"
#include "twCamera.h"

#define MAX_SCENE_CAMERA_COUNT 128

namespace TwinkleGraphics
{
class Scene final: public Object
{
public:
    typedef std::shared_ptr<Scene> Ptr;

    Scene();
    virtual ~Scene();

    void Init();
    void Update();
    void Render();

    void SetMainCamera(Camera::Ptr cam)
    { _maincamera = cam; }

    void AddCamera(Camera::Ptr cam)
    {
        if(cam == nullptr)
            return;
        _cameralists[++_valid_camera_count] = cam;
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

private:
    Camera::Ptr _maincamera;
    Camera::Ptr _cameralists[MAX_SCENE_CAMERA_COUNT];
    Transform::Ptr _sceneroot;

    int32 _valid_camera_count;
};  
} // namespace TwinkleGraphics

#endif