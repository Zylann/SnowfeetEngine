#ifndef __HEADER_SN_SERIALIZATIONCONTEXT__
#define __HEADER_SN_SERIALIZATIONCONTEXT__

#include <string>

namespace sn
{

/// \brief Contains data global to a serialization process.
class SN_API SerializationContext
{
public:
    SerializationContext(const std::string & moduleName) :
        m_module(moduleName)
    {}

    /// \brief In which module the data is currently serialized
    const std::string & getModule() const { return m_module; }

    /// \brief Gets an object from its ID in the serialized data.
    /// This is useful only in cases your object references others.
    ///
    /// \note This method depends on the type of data being deserialized.
    /// if it's a scene, then there is an instantiate pass and objects may be entities.
    /// In other cases, there might be no objects returned at all.
    ///
    template<class T>
    T * getObject(u32 objectRefID)
    {
        auto it = m_objects.find(objectRefID);
        if (it == m_objects.end())
            return nullptr;
        sn::Object * obj = it->second;
        return sn::Object::cast<T>(getObject());
    }

    std::unordered_map<u32, sn::Object*> & getObjectMap()
    {
        return m_objects;
    }

private:
    std::string m_module;
    std::unordered_map<u32, sn::Object*> m_objects;

};

} // namespace sn

#endif // __HEADER_SN_SERIALIZATIONCONTEXT__

