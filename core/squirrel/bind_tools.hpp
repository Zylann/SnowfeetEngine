/*
bind_tools.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SQ_BIND_TOOLS__
#define __HEADER_SN_SQ_BIND_TOOLS__

#include <squirrel.h>
#include <core/app/ScriptObject.hpp>

// TODO Use template magic to auto-do this when the class inherits ScriptObject?
#define SN_SQ_DEFINE_SCRIPTOBJECT_VARTYPE(_className)                         \
    namespace Sqrat {                                                         \
        template<>                                                            \
        struct Var<_className*> {                                             \
            _className* value;                                                \
            Var(HSQUIRRELVM vm, SQInteger idx) :                              \
                value(ClassType<_className>::GetInstance(vm, idx, true))      \
            {}                                                                \
            static void push(HSQUIRRELVM vm, _className* value) {             \
                value->pushSquirrelObject(vm);                                \
                sq_setreleasehook(                                            \
                    vm, -1, &sqRefCountedReleaseHook<_className>              \
                );                                                            \
            }                                                                 \
        };                                                                    \
    }

namespace Sqrat
{

/// \brief Specialization of the default allocator for RefCounted types.
/// For more detail, see Sqrat::DefaultAllocator<C>
template<class C>
class RefCountedAllocator : DefaultAllocator<C> {

public:

    static void SetInstance(HSQUIRRELVM vm, SQInteger idx, C* ptr)
    {
        ClassData<C>* cd = ClassType<C>::getClassData(vm);
        sq_setinstanceup(vm, idx, new std::pair<C*, SharedPtr<typename unordered_map<C*, HSQOBJECT>::type> >(ptr, cd->instances));
        sq_setreleasehook(vm, idx, &Delete);
        sq_getstackobj(vm, idx, &((*cd->instances)[ptr]));
    }

    static SQInteger New(HSQUIRRELVM vm) {
        SetInstance(vm, 1, NewC<C, is_default_constructible<C>::value >().p);
        return 0;
    }

    static SQInteger iNew(HSQUIRRELVM vm) {
        return New(vm);
    }

    template <typename A1>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
    template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    static SQInteger iNew(HSQUIRRELVM vm) {
        SQTRY()
            Var<A1> a1(vm, 2);
        Var<A2> a2(vm, 3);
        Var<A3> a3(vm, 4);
        Var<A4> a4(vm, 5);
        Var<A5> a5(vm, 6);
        Var<A6> a6(vm, 7);
        Var<A7> a7(vm, 8);
        Var<A8> a8(vm, 9);
        Var<A9> a9(vm, 10);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SetInstance(vm, 1, new C(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            a9.value
            ));
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

public:

    static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {
        SetInstance(vm, idx, new C(*static_cast<const C*>(value)));
        return 0;
    }

    static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
        SQUNUSED(size);
        auto * instance = reinterpret_cast<std::pair<C*, SharedPtr<typename unordered_map<C*, HSQOBJECT>::type> >*>(ptr);
        instance->second->erase(instance->first);
        instance->first->release();
        delete instance;
        return 0;
    }
};

template <class RefCounted_T>
SQInteger sqRefCountedReleaseHook(SQUserPointer ptr, SQInteger size)
{
    auto * instance = reinterpret_cast<std::pair<RefCounted_T*, SharedPtr<typename unordered_map<RefCounted_T*, HSQOBJECT>::type> >*>(ptr);
    instance->first->release();
    ClassType<RefCounted_T>::DeleteInstance(ptr, size);
    return 0;
}

/// \brief Prints the Squirrel stack in the console output
void sqDebugStackDump(HSQUIRRELVM v);

} // namespace Sqrat

#endif // __HEADER_SN_SQ_BIND_TOOLS__


