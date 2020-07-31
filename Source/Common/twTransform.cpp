

#include "twTransform.h"

namespace TwinkleGraphics
{
void Transform::Translate(glm::vec3 position)
{
    _position += position;
    _localDirty = _worldDirty = true;
}

void Transform::Translate(float trans, glm::vec3 axis)
{
    _position += glm::normalize(axis) * trans;
    _localDirty = _worldDirty = true;
}

void Transform::Rotate(float32 angle, glm::vec3 axis)
{
    glm::quat new_quat(::cosf(angle * 0.5f), ::sinf(angle * 0.5f) * glm::normalize(axis));
    _orientation = glm::normalize(new_quat * _orientation);
    _localDirty = _worldDirty = true;
}

void Transform::Rotate(glm::quat quaternion)
{
    _orientation = glm::normalize(quaternion * _orientation);
    _localDirty = _worldDirty = true;
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
    _localDirty = _worldDirty = true;
}

void Transform::Scale(glm::vec3 scale)
{
    _scale *= scale;
    _localDirty = _worldDirty = true;
}

void Transform::Scale(float scale, glm::vec3 axis)
{
    _localDirty = _worldDirty = true;
}

void Transform::LookAt(glm::vec3 center, glm::vec3 up)
{
    glm::mat4 mat_lookat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), center, up);
    _orientation = glm::quat_cast(mat_lookat);

    _localDirty = _worldDirty = true;
}

void Transform::Reset()
{
    _position = glm::vec3();
    _orientation = glm::identity<glm::quat>();
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);

    _localDirty = _worldDirty = true;
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

glm::quat Transform::GetWorldToLocalOrientation()
{
    if(_parent == nullptr)
    {
        return _orientation;
    }
    else
    {
        return _parent->GetWorldToLocalOrientation() * _orientation;
    }
}

glm::quat Transform::GetLocalToWorldOrientation()
{
    if(_parent == nullptr)
    {
        return glm::inverse(_orientation);
    }
    else
    {
        return glm::inverse(_orientation) * _parent->GetLocalToWorldOrientation();
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
        result = _localMatrix * _parent->GetWorldToLocalMatrix();
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
    if(WorldDirty())
    {
        ComputeLocalMatrix();

        if(_parent != nullptr)
        {
            _worldMatrix = _parent->GetLocalToWorldMatrix() * glm::inverse(_localMatrix);
        }
        else
        {
            _worldMatrix = glm::inverse(_localMatrix);
        }
        
        _worldDirty = false;
    }
}

void Transform::AddChild(Object::Ptr child)
{
    if(child != nullptr)
    {
        _children.push_back(child);
    }
}

void Transform::RemoveChild(Object::Ptr child)
{
    if(child != nullptr)
    {
        int32 pos = -1;
        for(auto c : _children)
        {
            pos++;
            if(c == child)
            {
                _children.erase(_children.begin() + pos);
                break;
            }
        }
    }
}

} // namespace TwinkleGraphics
