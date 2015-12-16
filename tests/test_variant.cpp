#include "tests.hpp"

#include <core/util/Variant.h>

using namespace sn;

static void printVariant(const Variant & v, std::ostream & os, int depth=0)
{
	for (int i = 0; i < depth; ++i)
		os << "    ";

	os << toString(v) << std::endl;

	if (v.isArray())
	{
		const Variant::Array & a = v.getArray();
		for (size_t i = 0; i < a.size(); ++i)
		{
			printVariant(v[i], os, depth + 1);
		}
	}
	else if (v.isDictionary())
	{
		const Variant::Dictionary & dict = v.getDictionary();
		for (auto it = dict.begin(); it != dict.end(); ++it)
		{
			printVariant(it->second, os, depth + 1);
		}
	}
}

void test_variant()
{
	Variant doc;

	doc.setDictionary();
	doc["hello"] = "world";
	doc["world"] = 42;

	Variant v;
	v.setArray();
	v[0] = 1.f;
	v[1] = 1.f;
	v[2] = 1.f;
	doc["position"].grab(v);

	if (doc["position"].isArray())
	{
		const auto & a = doc["position"].getArray();
		for (auto it = a.begin(); it != a.end(); ++it)
		{
			std::cout << "Hello " << toString(*it) << std::endl;
		}
	}

	// Without double lookup
	{
		const Variant & temp = doc["position"];
		if (temp.isArray())
		{
			const auto & a = temp.getArray();
			for (auto it = a.begin(); it != a.end(); ++it)
			{
				std::cout << "Hello again " << toString(*it) << std::endl;
			}
		}
	}

	printVariant(doc, std::cout);
}

