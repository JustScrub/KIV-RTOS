#include <stdfile.h>
#include <stdalloc.h>
#include <memory/memmap.h>

int main(int argc, char** argv)
{
    char* ptr = (char*)malloc(mem::PageSize);
    for(int i = 0; i < mem::PageSize+1; i++)
        ptr[i] = 'a' + (i % 26);

    while(1);
}