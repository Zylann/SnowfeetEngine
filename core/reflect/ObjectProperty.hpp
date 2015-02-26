#ifndef __HEADER_SN_OBJECTPROPERTY__
#define __HEADER_SN_OBJECTPROPERTY__

#include <functional>
#include <core/types.hpp>
#include <core/util/Log.hpp>

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
	void setGetterMethod(T(C::*method)())
	{
		m_getterTypeID = typeid(T);
		m_getterClosure = [](C* obj, void* data){
			*(T*)data = (obj->*method)();
		};
	}

	template <class C, typename T>
	void setSetterMethod(void(C::*method)(T))
	{
		m_setterTypeID = typeid(T);
		m_setterClosure = [](C* obj, void* data){
			(obj->*method)(*(T*)data);
		};
	}

	bool hasGetter() const { return (bool)m_getterClosure; }
	bool hasSetter() const { return (bool)m_setterClosure; }

	template <typename T>
	T getValue(Object * obj)
	{
		SN_ASSERT(m_getterTypeID == typeid(T), "ObjectProperty::getValue(): type mismatch for property " << toString());
		SN_ASSERT(m_getterClosure, "ObjectProperty::getValue(): no getter found for property " << toString());
		T data;
		m_getterClosure(obj, &data);
		return data;
	}

	template <typename T>
	void setValue(Object * obj, T data)
	{
		SN_ASSERT(m_setterTypeID == typeid(T), "ObjectProperty::setValue(): type mismatch for property " << toString());
		SN_ASSERT(m_setterClosure, "ObjectProperty::setValue(): no setter found for property " << toString());
		m_setterClosure(obj, &data);
	}

	std::string toString() const
	{
		std::string s = "{" + m_name;
		if (hasGetter())
			s += " get";
		if (hasSetter())
			s += " set";
		return s + "}";
	}

private:
	std::string m_name;
	u32 m_getterTypeID;
	u32 m_setterTypeID;
	GetterClosure m_getterClosure;
	SetterClosure m_setterClosure;
};

} // namespace sn

#endif // __HEADER_SN_OBJECTPROPERTY__

