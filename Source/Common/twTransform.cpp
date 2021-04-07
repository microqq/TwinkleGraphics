

#include "twTransform.h"
#include "twSceneNode.h"

namespace TwinkleGraphics
{
    void Transform::Translate(glm::vec3 position)
    {
        _position += position;
        _localDirty = true;
    }

    void Transform::Translate(float trans, glm::vec3 axis)
    {
        _position += glm::normalize(axis) * trans;
        _localDirty = true;
    }

    void Transform::Rotate(float32 angle, glm::vec3 axis)
    {
        glm::quat new_quat(::cosf(angle * 0.5f), ::sinf(angle * 0.5f) * glm::normalize(axis));
        _orientation = glm::normalize(new_quat * _orientation);
        _localDirty =  true;
    }

    void Transform::Rotate(glm::quat quaternion)
    {
        _orientation = glm::normalize(quaternion * _orientation);
        _localDirty = true;
    }

    /**
 * @brief 
 * euler:(x, y, z)-(pitch, yaw, roll)
 * @param euler 
 */
    void Transform::Rotate(glm::vec3 euler)
    {
        glm::quat euler_quat = glm::quat(euler);
        _orientation = glm::normalize(glm::inverse(euler_quat) * _orientation);
        _localDirty = true;
    }

    void Transform::Scale(glm::vec3 scale)
    {
        _scale *= scale;
        _localDirty = true;
    }

    void Transform::Scale(float scale, glm::vec3 axis)
    {
        _localDirty = true;
    }

    void Transform::LookAt(glm::vec3 center, glm::vec3 up)
    {
        glm::mat4 mat_lookat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), center, up);
        _orientation = glm::quat_cast(mat_lookat);

        _localDirty = true;
    }

    void Transform::Reset()
    {
        _position = glm::vec3();
        _orientation = glm::identity<glm::quat>();
        _scale = glm::vec3(1.0f, 1.0f, 1.0f);

        _localDirty = true;
    }

    glm::vec3 Transform::GetWorldPosition()
    {
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans == nullptr)
        {
            return GetPosition();
        }
        else
        {
            glm::vec4 world_position = ownerParentTrans->GetLocalToWorldMatrix() * glm::vec4(_position, 1.0f);
            return glm::vec3(world_position);
        }
    }

    glm::quat Transform::GetWorldToLocalOrientation()
    {
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans == nullptr)
        {
            return _orientation;
        }
        else
        {
            return ownerParentTrans->GetWorldToLocalOrientation() * _orientation;
        }
    }

    glm::quat Transform::GetLocalToWorldOrientation()
    {
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans == nullptr)
        {
            return glm::inverse(_orientation);
        }
        else
        {
            return glm::inverse(_orientation) * ownerParentTrans->GetLocalToWorldOrientation();
        }
    }

    glm::vec3 Transform::GetWorldScale()
    {
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans == nullptr)
        {
            return GetScale();
        }
        else
        {
            return ownerParentTrans->GetWorldScale() * _scale;
        }
    }

    glm::mat4 Transform::GetWorldToLocalMatrix()
    {
        ComputeLocalMatrix();

        glm::mat4 result;
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans != nullptr)
        {
            result = _localMatrix * ownerParentTrans->GetWorldToLocalMatrix();
        }
        else
        {
            result = _localMatrix;
        }

        return result;
    }

    const glm::mat4 &Transform::GetLocalToWorldMatrix()
    {
        ComputeWorldMatrix();
        return _worldMatrix;
    }

    void Transform::Update()
    {
        ComputeLocalMatrix();
        ComputeWorldMatrix();
    }

    bool Transform::WorldDirty()
    {
        TransformPtr ownerParentTrans = _owner->GetParentTransform();
        if (ownerParentTrans == nullptr)
            return _localDirty;
        else
        {
            return ownerParentTrans->WorldDirty() || _localDirty;
        }
    }

    void Transform::ComputeLocalMatrix()
    {
        if (_localDirty)
        {
            glm::mat4 translate_matrix = glm::identity<glm::mat4>();
            translate_matrix = glm::translate(translate_matrix, -_position);

            glm::mat4 scale_matrix = glm::identity<glm::mat4>();
            scale_matrix = glm::scale(scale_matrix, 1.0f / _scale);

            glm::mat4 rotate_matrix = glm::mat4_cast(_orientation);
            _localMatrix = scale_matrix * rotate_matrix * translate_matrix;

            _localDirty = false;
        }
    }

    void Transform::ComputeWorldMatrix()
    {
        if (WorldDirty())
        {
            ComputeLocalMatrix();

            TransformPtr ownerParentTrans = _owner->GetParentTransform();
            if (ownerParentTrans != nullptr)
            {
                _worldMatrix = ownerParentTrans->GetLocalToWorldMatrix() * glm::inverse(_localMatrix);
            }
            else
            {
                _worldMatrix = glm::inverse(_localMatrix);
            }
        }
    }
} // namespace TwinkleGraphics
