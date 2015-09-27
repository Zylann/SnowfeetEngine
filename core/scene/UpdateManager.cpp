#include "UpdateManager.h"
#include <core/util/stringutils.h>
#include <core/util/macros.h>
#include <core/scene/Entity.h>
#include <core/scene/Scene.h>

namespace sn
{

//------------------------------------------------------------------------------
const char * UpdateManager::DEFAULT_LAYER = "<DefaultUpdate>";

//------------------------------------------------------------------------------
UpdateManager::UpdateManager()
{
}

//------------------------------------------------------------------------------
UpdateManager::~UpdateManager()
{
    clear();
}

//------------------------------------------------------------------------------
void UpdateManager::addEntity(Entity & e, const std::string layerName)
{
    std::unordered_set<Entity*> * entities = nullptr;

    auto it = m_updatableEntities.find(layerName);
    if (it == m_updatableEntities.end())
    {
        UpdateLayer * layer = getLayer(layerName);
        if (layer == nullptr)
        {
            layer = new UpdateLayer(layerName);
            m_layers.push_back(layer);
        }
        entities = &m_updatableEntities[layerName];
    }
    else
    {
        entities = &it->second;
    }

    SN_ASSERT(entities != nullptr, "Entity list must not be null");

    entities->insert(&e);
}

//------------------------------------------------------------------------------
void UpdateManager::removeEntity(Entity & e, const std::string layerName)
{
    if (layerName.empty())
    {
        // Layer was provided, directly go to it
        auto it = m_updatableEntities.find(layerName);
        if (it != m_updatableEntities.end())
        {
            auto & entities = it->second;
            entities.erase(&e);
        }
    }
    else
    {
        // Layer not provided, search all of them
        for (auto it = m_updatableEntities.begin(); it != m_updatableEntities.end(); ++it)
        {
            auto & entities = it->second;
            if (entities.erase(&e) != 0)
                break;
        }
    }
}

//------------------------------------------------------------------------------
void UpdateManager::update()
{
    auto layers = m_layers;
    SN_FOREACH(it, layers)
    {
        UpdateLayer & layer = **it;
        if (layer.enabled)
        {
            updateLayer(layer);
        }
    }
}

//------------------------------------------------------------------------------
void UpdateManager::updateLayer(const UpdateLayer & layer)
{
    auto entitiesIt = m_updatableEntities.find(layer.name);
    if (entitiesIt != m_updatableEntities.end())
    {
        auto entities = entitiesIt->second;

        SN_FOREACH(entityIt, entities)
        {
            Entity & e = **entityIt;
            if (e.getFlag(SN_EF_ENABLED))
            {
                if (!e.getFlag(SN_EF_FIRST_UPDATE))
                {
                    e.onFirstUpdate();
                    e.setFlag(SN_EF_FIRST_UPDATE, true);
                }
                e.onUpdate();
            }
        }
    }
}

//------------------------------------------------------------------------------
void UpdateManager::clear()
{
    SN_FOREACH(it, m_layers)
    {
        UpdateLayer * layer = *it;
        delete layer;
    }
    m_layers.clear();
}

//------------------------------------------------------------------------------
bool UpdateManager::getEntityLayer(Entity & e, std::string & out_layerName)
{
    for (auto it = m_updatableEntities.begin(); it != m_updatableEntities.end(); ++it)
    {
        const auto & entities = it->second;
        if (entities.find(&e) != entities.end())
        {
            out_layerName = it->first;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
UpdateLayer * UpdateManager::getLayer(const std::string & name) const
{
    SN_FOREACH(it, m_layers)
    {
        UpdateLayer * layer = *it;
        if (layer->name == name)
            return layer;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void UpdateManager::serialize(Variant & o)
{
    // TODO
    SN_ERROR("Not implemented yet");
}

//------------------------------------------------------------------------------
namespace
{
    void unserializeLayer(const Variant & o, std::vector<UpdateLayer*> & layers, std::unordered_set<std::string> & names)
    {
        if (o.isDictionary())
        {
            UpdateLayer * layer = new UpdateLayer();
            layer->unserialize(o);

            if (names.find(layer->name) != names.end())
            {
                SN_ERROR("Layer name used twice!");
                delete layer;
                return;
            }

            names.insert(layer->name);
            layers.push_back(layer);
        }
        else if (o.isArray())
        {
            auto & a = o.getArray();
            for (u32 i = 0; i < a.size(); ++i)
            {
                const Variant & elem = a[i];
                unserializeLayer(elem, layers, names);
            }
        }
    }
}

//------------------------------------------------------------------------------
void UpdateManager::unserialize(const Variant & o)
{
    if (o.isArray())
    {
        std::unordered_set<std::string> names;
        std::vector<UpdateLayer*> layers;

        unserializeLayer(o, layers, names);

        // TODO Try not to destroy layers and update them instead
        clear();
        m_layers = layers;
    }
}

} // namespace sn

