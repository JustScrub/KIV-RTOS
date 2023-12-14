#include <stdalloc.h>

uint32_t __stdalloc_mem_offset = 0;

inline int32_t sz2pgs(uint32_t size)
{
    return size / mem::PageSize + (bool)(size % mem::PageSize);
}

void *malloc(size_t size)
{
    uint32_t addr = Task_Heap_Start + __stdalloc_mem_offset;

    if(addr + size > Task_Heap_End || size == 0)
        return nullptr;

    int32_t pages = sz2pgs(__stdalloc_mem_offset + size) - sz2pgs(__stdalloc_mem_offset);

    if(pages > 0 && i_am_once_again_asking_for_your_memory_support(pages) == nullptr)
        return nullptr;
    __stdalloc_mem_offset += size;
    return (void *)addr;
}