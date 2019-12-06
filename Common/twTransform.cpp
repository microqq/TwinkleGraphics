

#include "twTransform.h"

namespace TwinkleGraphics
{
void Transform::Translate(glm::vec3 position)
{
    _position += position;
    _local_dirty = _world_dirty = true;
}

void Transform::Translate(float trans, glm::vec3 axis)
{
    _position += glm::normalize(axis) * trans;
    _local_dirty = _world_dirty = true;
}

void Transform::Rotate(float32 angle, glm::vec3 axis)
{
    glm::quat new_quat(::cosf(angle * 0.5f), ::sinf(angle * 0.5f) * glm::normalize(axis));
    _orientation = glm::normalize(new_quat * _orientation);
    _local_dirty = _world_dirty = true;
}

void Transform::Rotate(glm::quat quaternion)
{
    _orientation = glm::normalize(quaternion * _orientation);
    _local_dirty = _world_dirty = true;
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
    _local_dirty = _world_dirty = true;
}

void Transform::Scale(glm::vec3 scale)
{
    _scale *= scale;
    _local_dirty = _world_dirty = true;
}

void Transform::Scale(float scale, glm::vec3 axis)
{
    _local_dirty = _world_dirty = true;
}

void Transform::LookAt(glm::vec3 center, glm::vec3 up)
{
}

void Transform::Reset()
{
    _position = glm::vec3();
    _orientation = glm::identity<glm::quat>();
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);

    _local_dirty = _world_dirty = true;
}

glm::vec3 Transform::GetWorldPosition()
{
    if(_parent == nullptr)
    {
        return GetPosition();
    }
    else
    {
        glm::vec4 world_position =_parent->GetLocalToWorldMatrix() * glm::vec4(_position, 1.0f);
        return glm::vec3(world_position);
    }
}

glm::quat Transform::GetWorldOrientation()
{
    if(_parent == nullptr)
    {
        return GetOrientation();
    }
    else
    {
        return _parent->GetWorldOrientation() * _orientation;
    }
}

glm::vec3 Transform::GetWorldScale()
{
    if(_parent == nullptr)
    {
        return GetScale();
    }
    else
    {
        return _parent->GetWorldScale() * _scale;
    }
}

glm::mat4 Transform::GetWorldToLocalMatrix()
{
    ComputeLocalMatrix();

    glm::mat4 result;
    if(_parent != nullptr)
    {
        result = _local_matrix * _parent->GetWorldToLocalMatrix();
    }
    else
    {
        result = _local_matrix;
    }
    
    return result;
}

const glm::mat4 &Transform::GetLocalToWorldMatrix()
{
    ComputeWorldMatrix();
    return _world_matrix;
}

void Transform::ComputeLocalMatrix()
{
    if (_local_dirty)
    {
        glm::mat4 translate_matrix = glm::identity<glm::mat4>();
        translate_matrix = glm::translate(translate_matrix, -_position);

        glm::mat4 scale_matrix = glm::identity<glm::mat4>();
        scale_matrix = glm::scale(scale_matrix, 1.0f / _scale);
        
        glm::mat4 rotate_matrix = glm::mat4_cast(glm::inverse(_orientation));
        _local_matrix = scale_matrix * rotate_matrix * translate_matrix;

        _local_dirty = false;
    }
}

void Transform::ComputeWorldMatrix()
{
    if(WorldDirty())
    {
        ComputeLocalMatrix();

        if(_parent != nullptr)
        {
            _world_matrix = _parent->GetLocalToWorldMatrix() * glm::inverse(_local_matrix);
        }
        else
        {
            _world_matrix = glm::inverse(_local_matrix);
        }
        
        _world_dirty = false;
    }
}

} // namespace TwinkleGraphics
