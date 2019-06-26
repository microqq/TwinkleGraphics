

#include "twTransform.h"

namespace TwinkleGraphics
{
void Transform::Translate(glm::vec3 position)
{
    _position += position;
    _local_dirty = _world_dirty = true;
}

void Transform::Rotate(float32 angle, glm::vec3 axis)
{
    _orientation = _orientation * glm::quat(angle, glm::normalize(axis));
    _local_dirty = _world_dirty = true;
}

void Transform::Rotate(glm::quat quaternion)
{
    _orientation = _orientation * quaternion;
    _local_dirty = _world_dirty = true;
}

void Transform::Scale(glm::vec3 scale)
{
    _scale *= scale;
    _local_dirty = _world_dirty = true;
}

void Transform::SetRotation(glm::vec3 euler)
{
    _local_euler = euler;
    _local_dirty = _world_dirty = true;
}

bool Transform::ComputeLocalMatrix()
{
    if (_local_dirty)
    {
        _local_matrix = glm::mat4();
        glm::scale(_local_matrix, _scale);

        _local_dirty = false;
    }
}

bool Transform::ComputeWorldMatrix()
{
}

} // namespace TwinkleGraphics
