#pragma once

#include <hal/intdef.h>
#include <memory/memmap.h>
#include <process/process.h>
#include <stdfile.h>

uint32_t __stdalloc_mem_offset = 0;
uint32_t __stdalloc_first_alloc = 0;


void *malloc(size_t size);