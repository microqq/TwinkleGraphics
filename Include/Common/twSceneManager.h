

#ifndef TW_SCENEMANAGEMENT_H
#define TW_SCENEMANAGEMENT_H

#include "twCommon.h"
#include "twCamera.h"

#define MAX_SCENE_CAMERA_COUNT 128

namespace TwinkleGraphics
{

    class __TWCOMExport Scene : public Object
    {
    public:
        typedef std::shared_ptr<Scene> Ptr;

        Scene();
        virtual ~Scene();

        virtual void Update(float32 deltaTime = 0.0f);
        virtual void Init();
        virtual void Render();

        void PreRender();
        void PosRender();

        void SetMainCamera(Camera::Ptr cam)
        {
            _maincamera = cam;
            AddCamera(cam);
        }

        void AddCamera(Camera::Ptr cam)
        {
            if (cam == nullptr)
                return;

            if(_cameralists.size() == MAX_SCENE_CAMERA_COUNT)
            {
                return;
            }

            std::vector<Camera::Ptr>::iterator findIter = std::find_if(_cameralists.begin(), _cameralists.end(), [cam](Camera::Ptr ele)
            {
                return cam == ele;
            });
            if(findIter != _cameralists.end())
            {
                _cameralists.emplace_back(cam);
                _cameraSorted = false;
            }
        }

        void RemoveCamera(int32 index)
        {
            if (index < 0 || index >= MAX_SCENE_CAMERA_COUNT)
            {
                return;
            }

            _cameralists.erase(_cameralists.begin() + index);
        }

        void RemoveCamera(Camera::Ptr cam)
        {
            if(cam == nullptr)
            {
                return;
            }

            std::vector<Camera::Ptr>::iterator findIter = std::find_if(_cameralists.begin(), _cameralists.end(), [cam](Camera::Ptr ele)
            {
                return cam == ele;
            });
            if(findIter != _cameralists.end())
            {
                _cameralists.erase(findIter);
            }
        }

        Camera::Ptr GetMainCamera() { return _maincamera; }
        Camera::Ptr GetCamera(int32 index)
        {
            if (index < 0 || index >= MAX_SCENE_CAMERA_COUNT)
            {
                return nullptr;
            }
            return _cameralists[index];
        }

        void SortCamera() 
        {
            if(!_cameraSorted)
            {
                std::sort(_cameralists.begin(), _cameralists.end(), [](Camera::Ptr a, Camera::Ptr b)
                {
                    return a->GetDepth() < b->GetDepth();
                });

                _cameraSorted = true;
            }
        }

        int32 GetCameraCount() { return _cameralists.size(); }

    private:
        void Traverse();

    protected:
        std::vector<Camera::Ptr> _cameralists;
        Camera::Ptr _maincamera = nullptr;
        ISceneNode::Ptr _rootNode = nullptr;

        bool _cameraSorted = false;
    };

    class SceneManager : public IUpdatable, public INonCopyable, public IDestroyable
    {
    public:
        virtual ~SceneManager();

        /**
         * @brief 
         * Update() must execute in main thread
         */
        virtual void Update(float deltaTime = 0.0f) override {}
        virtual void Destroy() override {}

        void RenderScene();

    private:
        explicit SceneManager()
            : IUpdatable()
            , INonCopyable()
            , IDestroyable()
        {}

    private:
        std::vector<Scene::Ptr> _sceneLists;
        Scene::Ptr _currentScene = nullptr;
    };

} // namespace TwinkleGraphics

#endif