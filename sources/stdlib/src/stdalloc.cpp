#include <stdalloc.h>

void *malloc(size_t size)
{
    uint32_t addr = Task_Heap_Start + __stdalloc_mem_offset;

    if(size > mem::PageSize) // unsupported
    {
        return nullptr
    }

    if(__stdalloc_mem_offset + size > Max_Mem || size == 0)
        return nullptr;

    if(__stdalloc_first_alloc)
    {
        if(i_am_once_again_asking_for_your_memory_support() == nullptr)
            return nullptr;
        __stdalloc_first_alloc = false;
        __stdalloc_mem_offset += size;
        return (void *)addr;
    }

    bool need_page = __stdalloc_mem_offset % mem::PageSize + size > mem::PageSize;

    if(need_page)
    {
        if(i_am_once_again_asking_for_your_memory_support() == nullptr)
            return nullptr;
    }

    __stdalloc_mem_offset += size;
    return (void *)addr;
}