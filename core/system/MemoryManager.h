#ifndef __HEADER_SN_MEMORYMANAGER__
#define __HEADER_SN_MEMORYMANAGER__

#include <string>
#include <unordered_map>
#include <core/export.h>
// config.h is not included because it defines new and delete overrides,
// which musn't be used inside the MemoryManager itself

namespace sn
{

enum MemoryAllocationType
{
    SN_MAT_SINGLE = 0,
    SN_MAT_ARRAY
};

struct SN_API MemoryBlock
{
    void * ptr;
    size_t size; // Allocated size
    std::string file; // File containing allocation (TODO use const char*?)
    int line;  // Allocation line
    MemoryAllocationType allocationType; // Is the block an array allocation?

    MemoryBlock() :
        ptr(nullptr),
        size(0),
        line(0),
        allocationType(SN_MAT_SINGLE)
    {}
};

class SN_API MemoryManager
{
public:
    static MemoryManager & get();

    void* allocate(size_t size, MemoryAllocationType allocType, const char * file, int line);
    void free(void* ptr, MemoryAllocationType allocType, const char * file, int line);
    void nextDelete(const char * file, int line);

    void reportLeaks();

private:
    MemoryManager();
    ~MemoryManager();

private:
    std::unordered_map<void*, MemoryBlock> m_allocations;

};

} // namespace sn

#endif // __HEADER_SN_MEMORYMANAGER__


