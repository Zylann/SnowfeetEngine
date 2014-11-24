#ifndef __HEADER_SN_OBJECTTYPE__
#define __HEADER_SN_OBJECTTYPE__

#include <string>
#include <sstream>
#include <core/types.hpp>

namespace sn
{

typedef u16 ObjectTypeID;


/// \brief You can derive this class to store additional information when registering an ObjectType.
/// \see ObjectType, ObjectTypeDatabase
class SN_API IObjectTypeUserData
{
private:
    IObjectTypeUserData() {}
public:
    virtual ~IObjectTypeUserData() {}
};


/// \brief Meta-class of an object.
struct SN_API ObjectType
{
    /// \brief Unique numeric ID of the component.
    /// DO NOT modify (only done on type registration).
    /// The value of IDs may be generated sequentially, starting from 1.
    /// 0 means null type.
    ObjectTypeID ID;

    /// \brief Unique name of the type.
    std::string name;

    /// \brief Name of the base class.
    std::string baseName;

    /// \brief Optional name of the module this object type is registered in.
    std::string moduleName;

    /// \brief you can store additional type information here when you register a type with ObjectTypeDatabase.
    /// It will be destroyed automatically when the singleton lifetime ends,
    /// unless you destroy it yourself and set it to null before.
    IObjectTypeUserData * userData;

    ObjectType(
        const std::string & p_name,
        const std::string & p_baseName
    ) :
        ID(0), // null ID, until the type gets registered
        name(p_name),
        baseName(p_baseName),
        userData(nullptr)
    {}

    ~ObjectType()
    {
        if(userData)
            delete userData;
    }

    void print(std::ostream & os) const
    {
        os << "{[" << ID << "]" << name << " : " << baseName << "}";
    }

    std::string toString() const
    {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    bool is(const ObjectType & other, bool includeInheritance=true) const;

};

} // namespace sn

#endif // __HEADER_SN_OBJECTTYPE__

