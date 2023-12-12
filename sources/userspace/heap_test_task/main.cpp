#include <stdstring.h>
#include <stdfile.h>
#include <stdalloc.h>
#include <hal/intdef.h>
#include <memory/memmap.h>
#include <drivers/bridges/uart_defs.h>

int main(int argc, char** argv)
{
    uint32_t uarth = open("DEV:uart/0", NFile_Open_Mode::Write_Only);
    TUART_IOCtl_Params params;
	params.baud_rate = NUART_Baud_Rate::BR_115200;
	params.char_length = NUART_Char_Length::Char_8;
	ioctl(uarth, NIOCtl_Operation::Set_Params, &params);

    size_t alloc_sizes[] = {mem::PageSize, 1, 0, mem::PageSize-1, mem::PageSize/2, mem::PageSize/2,
                            12*mem::PageSize, mem::PageSize, 1};
    const char* allocs[] = {"alloc first ", "alloc 1 byte ", "alloc 0 ", "alloc page size -1 ", "alloc half first ", "alloc half second ",
                      "alloc 12 pages ", "alloc last ", "alloc beyond "};

    char sptr[16] = {0};
    char* ptr;
    write(uarth, "Heap test\r\n", 11);
    for (size_t i = 0; i < sizeof(alloc_sizes)/sizeof(size_t); i++)
    {
        bzero(sptr, 16);
        write(uarth, "Allocating ", 11);
        ptr = (char*)malloc(alloc_sizes[i]);
        write(uarth, allocs[i], strlen(allocs[i]));
        write(uarth, "\r\n", 2);
        itoa((uint32_t)ptr, sptr, 16);
        write(uarth, sptr, strlen(sptr));
        write(uarth, "\r\n", 2);

        for(int j = 0; j < alloc_sizes[i]; j++)
            ptr[j] = 'a' + (j % 26);
    }
}