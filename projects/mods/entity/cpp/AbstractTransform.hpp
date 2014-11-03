/*
AbstractTransform.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ABSTRACT_TRANSFORM__
#define __HEADER_SN_ABSTRACT_TRANSFORM__

#include <core/types.hpp>
#include <core/math/Matrix4.hpp>

#include "Component.hpp"

namespace sn
{

/// \brief Represent the location and disposition of an object in space.
/// Units are arbitrary. Generally, try to match physics and graphics,
/// because it's easier to work with and involves less computations.
/// A transform can be a single couple of coordinates, or a complex local/global
/// set of matrices, so the purpose of this interface is to allow you to define
/// your own.
/// The engine works in float 3D, so if for instance your game uses integer units for positions,
/// you can inherit your custom location from this class, so the engine knows where to pick it.
/// Do not add two transforms to an entity, it's ambiguous.
/// 
/// Use:
/// Remember that a Transform is a component, so you may define it like this:
/// class MyTransform : public AbstractTransform
///
/// \see Transform (built-in 3D transform)
class SN_API AbstractTransform : public Component
{
public:

    SN_ABSTRACT_OBJECT(sn::AbstractTransform, sn::Component)

    // Required

    virtual sn::Vector3f position() const = 0;
    virtual void setPosition(sn::Vector3f newPos) = 0;

    // Optional

    virtual sn::Vector3f scale() const { return sn::Vector3f(1,1,1); }
    virtual void setScale(sn::Vector3f newScale) {}
    
    // TODO use Quaternions for rotation

    /// \brief Gets the rotation of the object.
    /// \return angle in degrees.
    //virtual f32 rotation() const { return 0; }

    /// \brief Sets the rotation of this object.
    /// \param newRotation: angle in degrees.
    //virtual void setRotation(f32 degrees) {}

    /// \brief Returns the global transformation matrix of this transform.
    /// If it has no such matrix, it will get recomputed on the fly from above
    /// properties (by the graphics system mostly).
    virtual const sn::Matrix4 * matrix() const { return nullptr; }

    /// \brief Called when the parent of the entity owning the component
    /// has a new parent or has been unparented
    virtual void onParentChanged() {}

    // Delta transformations

    virtual void translate(sn::Vector3f motion);
    //virtual void rotate(f32 degrees);
    virtual void scale(sn::Vector3f scaleFactor);

};

} // namespace sn

#endif // __HEADER_SN_ABSTRACT_TRANSFORM__


