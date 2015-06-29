#ifndef __HEADER_SN_OBJECTPROPERTY__
#define __HEADER_SN_OBJECTPROPERTY__

#include <core/types.h>
#include <core/util/Log.h>
#include <core/reflect/TypeTraits.h>

#include <functional>
#include <typeindex>

namespace sn
{

class Object;

class SN_API ObjectProperty
{
public:
	typedef std::function<void(Object*, void*)> GetterClosure;
	typedef std::function<void(Object*, void*)> SetterClosure;

	ObjectProperty(const std::string & name) :
		m_name(name),
        m_getterTypeID(0),
        m_setterTypeID(0)
	{}

	template <class C, typename T>
	void setGetterMethod(const T&(C::*method)())
	{
		m_getterTypeID = getTypeID<T>();
		m_getterClosure = [=](Object* obj, void* data){
			*(T*)data = (((C*)obj)->*method)();
		};
	}

    template <class C, typename T>
    void setGetterMethod(T(C::*method)())
    {
        m_getterTypeID = getTypeID<T>();
        m_getterClosure = [=](Object* obj, void* data){
            *(T*)data = (((C*)obj)->*method)();
        };
    }

	template <class C, typename T>
	void setSetterMethod(void(C::*method)(T))
	{
        m_setterTypeID = getTypeID<T>();
        m_setterClosure = [=](Object* obj, void* data){
			(((C*)obj)->*method)(*(T*)data);
		};
	}

    template <class C, typename T>
    void setSetterMethod(void(C::*method)(const T&))
    {
        m_setterTypeID = getTypeID<T>();
        m_setterClosure = [=](Object* obj, void* data){
            (((C*)obj)->*method)(*(T*)data);
        };
    }

	bool hasGetter() const { return (bool)m_getterClosure; }
	bool hasSetter() const { return (bool)m_setterClosure; }

    template <typename T>
    bool is() const
    {
        return m_getterTypeID == getTypeID<T>() && m_setterTypeID == getTypeID<T>();
    }

	template <typename T>
	T getValue(Object * obj) const
	{
        SN_ASSERT(m_getterTypeID == getTypeID<T>(), "ObjectProperty::getValue(): type mismatch for property " << toString());
		SN_ASSERT(m_getterClosure, "ObjectProperty::getValue(): no getter found for property " << toString());
		T data;
		m_getterClosure(obj, &data);
		return data;
	}

	template <typename T>
	void setValue(Object * obj, T data) const
	{
        SN_ASSERT(m_setterTypeID == getTypeID<T>(), "ObjectProperty::setValue(): type mismatch for property " << toString());
		SN_ASSERT(m_setterClosure, "ObjectProperty::setValue(): no setter found for property " << toString());
		m_setterClosure(obj, &data);
	}

    inline const std::string & getName() const { return m_name; }

    std::string toString() const;

private:
	std::string m_name;
	u32 m_getterTypeID;
    u32 m_setterTypeID;
	GetterClosure m_getterClosure;
	SetterClosure m_setterClosure;
};

} // namespace sn

#endif // __HEADER_SN_OBJECTPROPERTY__

