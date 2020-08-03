

#ifndef TW_TRANSFORM_H
#define TW_TRANSFORM_H


#include <memory>
#include <vector>

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
        , _localMatrix(glm::identity<glm::mat4>())
        , _worldMatrix(glm::identity<glm::mat4>())
        , _orientation(glm::identity<glm::quat>())
        , _position(glm::vec3(0.0f, 0.0f, 0.0f))
        , _scale(glm::vec3(1.0f, 1.0f, 1.0f))
        , _parent(nullptr)
        , _localDirty(true)
        , _worldDirty(true)
    {}
    ~Transform() 
    {
        _children.clear();
    }

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
    glm::vec3 GetWorldScale();
    glm::quat GetWorldToLocalOrientation();
    glm::quat GetLocalToWorldOrientation();

    inline void SetPosition(glm::vec3 position) { _position = position; _localDirty = _worldDirty = true; }
    inline void SetOrientation(glm::quat orientation) { _orientation = orientation; _localDirty = _worldDirty = true; }
    inline void SetScale(glm::vec3 scale) { _scale = scale; _localDirty = _worldDirty = true; }

    glm::mat4 GetWorldToLocalMatrix();
    const glm::mat4 GetLocalMatrix() { return _localMatrix; }
    const glm::mat4& GetLocalToWorldMatrix();

    void SetParent(Ptr parent) 
    { 
        _parent = parent;
        _parent->AddChild(shared_from_this());
        _worldDirty = true; 
    }
    void RemoveFromParent()
    {
        _parent->RemoveChild(shared_from_this());
        _parent = nullptr; 
        _worldDirty = true;
    }
    Object::Ptr GetChild(int32 pos) 
    { 
        if(pos >= 0 && pos < _children.size())
        {
            return _children[pos];
        }

        return nullptr;
    }
    int32 GetChildrenCount() { return _children.size(); } 
    inline bool LocalDirty() { return _localDirty; }
    inline bool WorldDirty() 
    {
        if(_parent == nullptr)
            return _worldDirty; 
        else 
        {
            return _parent->WorldDirty() || _worldDirty;
        }
    }

private:
    void ComputeLocalMatrix();
    void ComputeWorldMatrix();
    void AddChild(Object::Ptr child);
    void RemoveChild(Object::Ptr child);

private:
    std::vector<Object::Ptr> _children;
    //transform relative to parent transform
    glm::mat4 _localMatrix;
    //world transform
    glm::mat4 _worldMatrix;

    //orientation relative to parent
    glm::quat _orientation;
    glm::vec3 _position;
    glm::vec3 _scale;

    Ptr _parent;

    bool _localDirty;
    bool _worldDirty;
};
} // namespace TwinkleGraphics

#endif
