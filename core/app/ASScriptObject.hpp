#ifndef __HEADER_SN_AS_SCRIPT_OBJECT__
#define __HEADER_SN_AS_SCRIPT_OBJECT__

#include <angelscript.h>
#include <core/util/RefCounted.hpp>

namespace sn
{

/// \brief Objects needing full binding to AngelScript may inherit this class.
/// It provides reference counting and a weak reference attribute if needed.
class SN_API ASScriptObject : public RefCounted
{
public:

    ASScriptObject(): RefCounted(), m_weakRefFlag(nullptr) {}

    /// \brief Gets the weakref flag used by AngelScript.
    /// Note that the weakref addon is required for it to be used in scripts.
    asILockableSharedBool *getWeakRefFlag()
    {
        if (!m_weakRefFlag)
            m_weakRefFlag = asCreateLockableSharedBool();
        return m_weakRefFlag;
    }

protected:

    // Note: use release() if you don't need the object anymore
    virtual ~ASScriptObject()
    {
        if (m_weakRefFlag)
        {
            // Tell the ones that hold weak references that the object is destroyed
            m_weakRefFlag->Set(true);
            m_weakRefFlag->Release();
        }
    }

private:

    asILockableSharedBool *m_weakRefFlag;

};

} // namespace sn

#endif // __HEADER_SN_AS_SCRIPT_OBJECT__

