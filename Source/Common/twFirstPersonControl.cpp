

#include "twFirstPersonControl.h"

namespace TwinkleGraphics
{
    FirstPersonControl::FirstPersonControl(CameraPtr camera)
        : OrbitControl(camera)
        , _stepX(1.0f)
        , _stepZ(1.0f)
    {
        _camera->Translate(glm::vec3(0.0f, 0.0f, -_distance));
    }

    FirstPersonControl::~FirstPersonControl()
    {
    }

    void FirstPersonControl::HandleKeyPress(int32 key, int32 scannode, int32 action, int32 mods)
    {
        TransformPtr targetTrans = _target->GetTransform();
        if (targetTrans != nullptr)
        {
            glm::mat3 rotateMat = glm::mat3_cast(_camera->GetTransform()->GetLocalToWorldOrientation());
            // glm::mat3 rotateMatInv = glm::inverse(rotateMat);

            switch (key)
            {
            case 87: // GLFW_KEY_W
            {
                vec3 zAxisOffset = (rotateMat * vec3(0.0f, 0.0f, -_stepZ));
                targetTrans->Translate(zAxisOffset);
            }
                break;
            case 83: // GLFW_KEY_S
            {
                vec3 zAxisOffset = (rotateMat * vec3(0.0f, 0.0f, +_stepZ));
                targetTrans->Translate(zAxisOffset);
            }
                break;
            case 65: // GLFW_KEY_A
            {
                vec3 xAxisOffset = (rotateMat * vec3(-_stepX, 0.0f, 0.0f));
                targetTrans->Translate(xAxisOffset);
            }
                break;
            case 68: // GLFW_KEY_D
            {
                vec3 xAxisOffset = (rotateMat * vec3(+_stepX, 0.0f, 0.0f));
                targetTrans->Translate(xAxisOffset);
            }
                break;
            default:
                break;
            }
        }
    }

    void FirstPersonControl::HandleKeyRelease(int32 key, int32 scannode, int32 action, int32 mods)
    {
    }
} // namespace TwinkleGraphics
