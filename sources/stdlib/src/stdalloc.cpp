#include <stdalloc.h>

void *malloc(size_t size)
{
    uint32_t addr = Task_Heap_Start + __stdalloc_mem_offset;

    if(addr + size > Task_Heap_End || size == 0)
        return nullptr;

    uint32_t pages = (__stdalloc_mem_offset+size) / mem::PageSize - __stdalloc_mem_offset / mem::PageSize;
    pages += __stdalloc_first_alloc; // at first alloc, we need to allocate one more page for heap start

    if(i_am_once_again_asking_for_your_memory_support(pages) == nullptr)
        return nullptr;
    __stdalloc_first_alloc = 0;
    __stdalloc_mem_offset += size;
    return (void *)addr;
}