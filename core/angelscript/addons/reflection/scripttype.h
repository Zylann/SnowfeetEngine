#ifndef __HEADER_AS_ADDON_SCRIPTTYPE__
#define __HEADER_AS_ADDON_SCRIPTTYPE__

#include <angelscript.h>
#include <assert.h>
#include <string>

BEGIN_AS_NAMESPACE

/// \brief underlying common class for Type and Type<T> script types
class CScriptType
{
public:

    // TODO m_ot can be null because AS doesn't allows value types with no default constructors yet...

    CScriptType(asIObjectType *type):
        m_ot(type)
    {
        if (m_ot)
            m_ot->AddRef();
    }

    CScriptType(asIObjectType *type, const CScriptType &other):
        m_ot(type)
    {
        if (m_ot)
        {
            m_ot->AddRef();
            assert(m_ot == other.m_ot);
        }
    }

    CScriptType(const CScriptType &other)
    {
        m_ot = other.m_ot;
        m_ot->AddRef();
    }

    ~CScriptType()
    {
        if (m_ot)
        {
            m_ot->Release();
        }
    }

    CScriptType &operator=(const CScriptType& other)
    {
        if (this != &other)
        {
            if (m_ot)
                m_ot->Release();
            m_ot = other.m_ot;
            if (m_ot)
                m_ot->AddRef();
        }

        return *this;
    }

    // Compare two types
    bool operator==(const CScriptType & other) const
    {
        return m_ot->GetTypeId() == other.m_ot->GetTypeId();
    }

    bool DerivesFrom(const CScriptType & other) const
    {
        if (m_ot)
            return m_ot->DerivesFrom(other.m_ot);
        return false;
    }

    std::string ToString() const
    {
        if (m_ot)
        {
            std::string ns = m_ot->GetNamespace();
            if (ns.empty())
                return m_ot->GetName();
            else
                return ns + "::" + m_ot->GetName();
        }
        else
        {
            return "<NULL>";
        }
    }

protected:
    asIObjectType *m_ot;
};

void RegisterScriptType(asIScriptEngine *engine);

END_AS_NAMESPACE

#endif // __HEADER_AS_ADDON_SCRIPTTYPE__

