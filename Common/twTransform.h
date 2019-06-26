

#ifndef TW_TRANSFORM_H
#define TW_TRANSFORM_H

#include "twCommon.h"

namespace TwinkleGraphics
{
class Transform final : public Object
{
public:
    typedef std::shared_ptr<Transform> Ptr;

    Transform()
        : Object()
        , _parent(nullptr)
        , _local_dirty(false)
        , _world_dirty(false)
    {}
    ~Transform() {}

    inline glm::vec3 GetPosition() { return _position; }
    inline glm::quat GetOrientation() { return _orientation; }
    inline glm::vec3 GetScale() { return _scale; }

    void Translate(glm::vec3 position);
    void Rotate(float32 angle, glm::vec3 axis);
    void Rotate(glm::quat quaternion);
    void Scale(glm::vec3 scale);

    inline void SetPosition(glm::vec3 position) { _position = position; _local_dirty = _world_dirty = true; }
    inline void SetOrientation(glm::quat orientation) { _orientation = orientation; _local_dirty = _world_dirty = true; }
    void SetRotation(glm::vec3 euler);
    inline void SetScale(glm::vec3 scale) { _scale = scale; _local_dirty = _world_dirty = true; }

    const glm::mat4& GetLocalMatrix();
    const glm::mat4& GetWorldMatrix();

    inline void SetParent(Ptr parent) { _parent = parent; _world_dirty = true; }
    inline void RemoveFromParent() { _parent = nullptr; _world_dirty = true; }
    inline bool LocalDirty() { return _local_dirty; }
    inline bool WorldDirty() { return _world_dirty; }

private:
    bool ComputeLocalMatrix();
    bool ComputeWorldMatrix();

private:
    glm::mat4 _local_matrix;
    glm::mat4 _world_matrix;

    glm::quat _orientation;
    glm::vec3 _position;
    glm::vec3 _local_euler;
    glm::vec3 _scale;

    Ptr _parent;

    bool _local_dirty;
    bool _world_dirty;
};
} // namespace TwinkleGraphics

#endif
