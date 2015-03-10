//#include "../../util/assert.hpp"
#include "MemoryManager.hpp"
#include <iostream>
#include <cassert>
#include <exception> // for std::bad_alloc
#include <new>
#include <cstdlib> // for malloc() and free()

namespace sn
{

MemoryManager & MemoryManager::get()
{
    static MemoryManager s_instance;
    return s_instance;
}

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}

void* MemoryManager::allocate(size_t size, MemoryAllocationType allocType, const char * file, int line)
{
    void * ptr = ::malloc(size);
    if (ptr == nullptr)
        throw std::bad_alloc(); // ANSI/ISO compliant behavior

    MemoryBlock newBlock;
    newBlock.ptr = ptr;
    newBlock.size = size;
    newBlock.file = file;
    newBlock.line = line;
    newBlock.allocationType = allocType;
    m_allocations[ptr] = newBlock;

    return ptr;
}

void MemoryManager::free(void* ptr, MemoryAllocationType allocType, const char * file, int line)
{
    auto it = m_allocations.find(ptr);

    if (it == m_allocations.end())
    {
        ::free(ptr);
        return;
    }

    assert(it->second.allocationType == allocType);

    m_allocations.erase(it);

    ::free(ptr);
}

void MemoryManager::nextDelete(const char * file, int line)
{

}

void MemoryManager::reportLeaks()
{
    if (m_allocations.empty())
    {
        std::cout << "No leak detected." << std::endl;
    }
    else
    {
        std::cout << ("Leaks detected!") << std::endl;
        size_t totalSize = 0;

        for (auto it = m_allocations.begin(); it != m_allocations.end(); ++it)
        {
            const MemoryBlock & b = it->second;

            totalSize += b.size;

            std::cout << "-> 0x" << it->first << ": " << b.size << " bytes in " << ":" << b.line << std::endl;

            //::free(it->first);
        }

        std::cout << "Total leaks: " << m_allocations.size() << " blocks, " << totalSize << " bytes" << std::endl;
    }
}

} // namespace sn

