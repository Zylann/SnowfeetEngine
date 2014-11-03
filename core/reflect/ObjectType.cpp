#include "ObjectType.hpp"
#include "ObjectTypeDatabase.hpp"

namespace sn
{

bool ObjectType::is(const ObjectType & other, bool includeInheritance) const
{
    // If the object has a Null type
    if(ID == 0)
    {
        // Always return false
        return false;
    }

    if(ID == other.ID)
    {
        // Types exactly match
        return true;
    }
    else if(includeInheritance)
    {
        // Search inheritance tree

        ObjectTypeDatabase & odb = ObjectTypeDatabase::get();
        const ObjectType * baseType = this;

        while(!baseType->baseName.empty())
        {
            baseType = odb.getType(baseType->baseName);
            if(baseType == nullptr)
                return false;
            if(baseType->ID == other.ID)
                return true;
        }
    }

    // Types differ
    return false;
}

} // namespace sn

