

#ifndef TW_TRANSFORM_H
#define TW_TRANSFORM_H

#include "twCommon.h"

namespace TwinkleGraphics
{
enum class Space
{
    SELF,
    PARENT,
    WORLD
};

class Transform final : public Object
{
public:
    typedef std::shared_ptr<Transform> Ptr;

    Transform()
        : Object()
        , _local_matrix(glm::identity<glm::mat4>())
        , _world_matrix(glm::identity<glm::mat4>())
        , _orientation(glm::identity<glm::quat>())
        , _position()
        , _scale(glm::vec3(1.0f, 1.0f, 1.0f))
        , _parent(nullptr)
        , _local_dirty(false)
        , _world_dirty(false)
    {}
    ~Transform() {}

    void Translate(glm::vec3 position);
    void Translate(float trans, glm::vec3 axis);
    void Rotate(float32 angle, glm::vec3 axis);
    void Rotate(glm::quat quaternion);
    void Rotate(glm::vec3 euler);
    void Scale(glm::vec3 scale);
    void Scale(float scale, glm::vec3 axis);
    void LookAt(glm::vec3 center, glm::vec3 up);
    void Reset();

    inline glm::vec3 GetPosition() { return _position; }
    inline glm::quat GetOrientation() { return _orientation; }
    inline glm::vec3 GetEuler() { return glm::eulerAngles(_orientation); }
    inline glm::vec3 GetScale() { return _scale; }

    glm::vec3 GetWorldPosition();
    glm::quat GetWorldOrientation();
    glm::vec3 GetWorldScale();

    inline void SetPosition(glm::vec3 position) { _position = position; _local_dirty = _world_dirty = true; }
    inline void SetOrientation(glm::quat orientation) { _orientation = orientation; _local_dirty = _world_dirty = true; }
    inline void SetScale(glm::vec3 scale) { _scale = scale; _local_dirty = _world_dirty = true; }

    glm::mat4 GetWorldToLocalMatrix();
    const glm::mat4 GetLocalMatrix() { return _local_matrix; }
    const glm::mat4& GetLocalToWorldMatrix();

    inline void SetParent(Ptr parent) { _parent = parent; _world_dirty = true; }
    inline void RemoveFromParent() { _parent = nullptr; _world_dirty = true; }
    inline bool LocalDirty() { return _local_dirty; }
    inline bool WorldDirty() { return _world_dirty; }

private:
    void ComputeLocalMatrix();
    void ComputeWorldMatrix();

private:
    //transform relative to parent transform
    glm::mat4 _local_matrix;
    //world transform
    glm::mat4 _world_matrix;

    //orientation relative to parent
    glm::quat _orientation;
    glm::vec3 _position;
    glm::vec3 _scale;

    Ptr _parent;

    bool _local_dirty;
    bool _world_dirty;
};
} // namespace TwinkleGraphics

#endif
