/*
Transform.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_TRANSFORM__
#define __HEADER_SN_TRANSFORM__

#include <core/json/json_utils.hpp>

#include "AbstractTransform.hpp"

namespace sn
{

class Entity;

/// \brief Built-in float transform containing a local and global set of
/// position, scale and rotations.
/// It also contains entity hierarchy information.
/// \note In the future, the hierarchy might move to Entity directly.
class SN_API Transform : public AbstractTransform
{
public:

    SN_OBJECT(sn::Transform, sn::AbstractTransform)

//	typedef std::vector<Transform*> container;
//	typedef container::iterator iterator;
//	typedef container::const_iterator const_iterator;

    Transform();

    //-------------------------
    // Transformation
    //-------------------------

    // Local transform accessors

    sn::Vector3f localPosition() const;
    sn::Vector3f localScale() const;
    //f32 localRotation() const; // in degrees

    void setLocalPosition(const sn::Vector3f & newPosition);
    void setLocalScale(const sn::Vector3f & newScale);
    //void setLocalRotation(f32 newRotation); // in degrees

    // Global transform accessors
    // Important note: these are a lot slower than local ones,
    // because the hierarchy transforms gets recomputed each time.
    // Use them carefully (get them in temporary variables for instance).

    sn::Vector3f position() const override;
    sn::Vector3f scale() const override;
    //f32 rotation() const override;

    void setPosition(sn::Vector3f newPosition) override;
    void setScale(sn::Vector3f newScale) override;
    //void setRotation(f32 newRotation) override; // in degrees

    void translate(sn::Vector3f motion) override;
    void scale(sn::Vector3f scaleFactor) override;
    //void rotate(f32 degrees) override;

    // Matrices

    const sn::Matrix4 & localMatrix() const;
    const sn::Matrix4 * matrix() const override;

    //-------------------------
    // Hierarchy
    //-------------------------

    void onParentChanged() override;

    // Get parent transform. Returns null if the parent entity has no transform.
    AbstractTransform * parent() const;

    // Sets the parent transform. Also adds the transform to the specified parent's children list.
    // Specifying a nullptr parent means unparenting (set back to root).
//	void setParent(Transform * parent);

    // Unparents every child under this transform.
//	void unparentChildren();

    // Reference version of setParent(Transform*) for convenience
//	inline void setParent(Transform & parent) { setParent(&parent); }

    // Gets the number of children parented to this transform
//	inline u32 childCount() const { return m_children.size(); }

    // Gets a child at the given index (must be valid).
//	Transform * child(u32 index) const;

    // Iterators to children

//	inline iterator begin()               { return m_children.begin(); }
//	inline iterator end()                 { return m_children.end(); }
//	inline const_iterator cbegin() const  { return m_children.cbegin(); }
//	inline const_iterator cend() const    { return m_children.cend(); }

    //-------------------------
    // Serialization
    //-------------------------

    void serializeData(JsonBox::Value & o) override;
    void unserializeData(JsonBox::Value & o) override;
    //void postUnserialize(JsonBox::Value & o) override;

private:

    // Tells children to update their global matrix when needed
    void notifyChildrenForParentChange();

    // Called by setParent(transform) from another Transform
//	void onAddChild(Transform * child);

    // Called by setParent(nullptr) from another Transform
//	void onRemoveChild(Transform * child);

//	Transform * r_parent;
//	container m_children;

    // Local decomposed transformation

    sn::Vector3f m_position;
    sn::Vector3f m_scale;
    f32 m_rotation;

    // Matrices

    mutable sn::Matrix4 m_localMatrix;
    mutable sn::Matrix4 m_globalMatrix;

    mutable bool m_localMatrixNeedUpdate;
    mutable bool m_globalMatrixNeedUpdate;

};

} // namespace sn

#endif // __HEADER_SN_TRANSFORM_HPP__


