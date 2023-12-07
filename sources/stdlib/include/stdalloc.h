#pragma once

#include <hal/intdef.h>
#include <memory/memmap.h>
#include <process/process.h>
#include <stdfile.h>

constexpr uint32_t Max_Mem = Task_Max_Heap_Pages * mem::PageSize;
constexpr uint32_t MALLOC_MAX_SIZE = mem::PageSize;
uint32_t __stdalloc_mem_offset = 0;
bool __stdalloc_first_alloc = true;


void *malloc(size_t size);