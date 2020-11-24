#include "twSceneNode.h"

namespace TwinkleGraphics
{
    SceneNode::~SceneNode()
    {
        _children.clear();
    }

    void SceneNode::Update(float deltaTime)
    {
        _transform->Update();
    }

    void SceneNode::AddChild(SceneNode::Ptr node)
    {
        if(HasChild(node))
        {
            return;
        }

        SceneNode::Ptr oldParent = node->_parent;
        if(oldParent != nullptr)
        {
            oldParent->RemoveChild(node);
        }
        _children.emplace_back(node);

        SceneNode::Ptr thisParent = std::dynamic_pointer_cast<SceneNode>(shared_from_this());
        node->_parent = thisParent;
        node->_transform->MarkDirty();
    }

    void SceneNode::RemoveChild(SceneNode::Ptr node)
    {
        Iterator iter = FindChild(node);
        if(iter == _children.end())
        {
            return;
        }

        _children.erase(iter);
        node->_parent = nullptr;
        node->_transform->MarkDirty();
    }

    void SceneNode::AttachToParent(SceneNode::Ptr parent)
    {
        if(parent == nullptr || _parent == parent)
        {
            return;
        }

        SceneNode::Ptr thisNode = std::dynamic_pointer_cast<SceneNode>(shared_from_this());
        parent->AddChild(thisNode);
    }

    void SceneNode::DetachFromParent()
    {
        if(_parent == nullptr)
        {
            return;
        }
        
        SceneNode::Ptr thisNode = std::dynamic_pointer_cast<SceneNode>(shared_from_this());
        _parent->RemoveChild(thisNode);
    }

    void SceneNode::Traverse()
    {
        for(auto node : _children)
        {

        }
    }

    bool SceneNode::HasChild(SceneNode::Ptr node)
    {
        return FindChild(node) != _children.end();
    }

    SceneNode::Iterator SceneNode::FindChild(SceneNode::Ptr node)
    {
        if(node == nullptr)
        {
            return _children.end();
        }

        Iterator iter = std::find_if(_children.begin(), _children.end()
            , [node](SceneNode::Ptr child)
            {
            return child == node;
            }
        );

        return iter != _children.end() ? iter : _children.end();
    }

} // namespace TwinkleGraphics
