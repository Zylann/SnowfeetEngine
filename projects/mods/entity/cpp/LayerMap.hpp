/*
LayerMap.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_LAYERMAP__
#define __HEADER_SN_LAYERMAP__

#include <iostream>
#include <unordered_map>
#include <cassert>

#include <core/types.hpp>
#include <core/util/Log.hpp>

namespace sn
{

/// \brief A layer is a group of entities withing the scene.
/// This structure only stores layer's infos.
struct Layer
{
    /// \brief Name of the layer, recommended to be able to identify it.
    std::string name;

    /// \brief Draw order of the layer. It goes over renderer's draw orders.
    s32 drawOrder;

    /// \brief Index of the layer, used in camera masks (Do not mix up with drawOrder).
    u8 index;

    Layer() :
        name(""),
        drawOrder(0),
        index(0)
    {}

};

/// \brief A set of 32 named layers.
/// This number is fixed to allow camera masks to be encoded on unsigned integers.
class SN_API LayerMap
{
public:

    static const u32 COUNT = 32;

    /// \brief Constructs the default set of layers.
    /// The layer 0 has the name "default".
    LayerMap()
    {
        for(u8 i = 0; i < COUNT; ++i)
        {
            m_layers[i].index = i;
        }

        m_layers[0].name = "default";
    }

    /// \brief Sets the attributes of a layer.
    /// \param index: index of the layer, between 0 and 31 included.
    /// \param name: name of the layer.
    /// \param drawOrder: drawOrder of the layer.
    inline void setLayer(u8 index, std::string name, s32 drawOrder=0)
    {
        assert(index < COUNT);
        m_layers[index].name = name;
        m_layers[index].drawOrder = drawOrder;
    }

    /// \brief Accesses the layer from an index.
    /// \param index: index of the layer, between 0 and 31 included.
    /// \return reference to the layer.
    Layer & operator[](u32 index)
    {
        assert(index < COUNT);
        return m_layers[index];
    }

    /// \brief Accesses the layer having the given name.
    /// \param name: name of the layer to search
    /// \return pointer to the layer, or nullptr if not found.
    Layer * operator[](const std::string & name)
    {
        for(u32 i = 0; i < COUNT; ++i)
        {
            if(m_layers[i].name == name)
            {
                return &(m_layers[i]);
            }
        }
#ifdef SN_BUILD_DEBUG
        SN_ERROR("LayerMap::[name]: layer not found \"" << name << '"');
#endif
        return nullptr;
    }

    /// \brief Get the mask corresponding to a layer.
    /// \param name: name of the layer
    /// \return Bitmask where one bit is set on the layer's position,
    /// or 0 if no layer was found with the given name.
    u32 maskFromName(const std::string & name)
    {
        for(u32 i = 0; i < COUNT; ++i)
        {
            if(m_layers[i].name == name)
            {
                return 1 << i;
            }
        }
#ifdef SN_BUILD_DEBUG
        SN_ERROR("LayerMap::maskFromName: layer not found \"" << name << '"');
#endif
        return 0;
    }

    /// \brief Prints the LayerMap in a human-readable form.
    void print(std::ostream & os) const
    {
        for(u32 i = 0; i < COUNT; ++i)
        {
            const Layer & l = m_layers[i];
            SN_DLOG("[" << i << "]: \"" << l.name << "\", drawOrder=" << l.drawOrder);
        }
    }

    // TODO serialize layers

private:

    /// \brief All the 32 layers.
    Layer m_layers[COUNT];

};

} // namespace sn

#endif // __HEADER_SN_LAYERMAP__

