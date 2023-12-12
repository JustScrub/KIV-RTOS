#include <stdstring.h>
#include <stdfile.h>
#include <stdmutex.h>

#include <drivers/bridges/uart_defs.h>
#include <process/process.h>

#define TASK_DEADLINE 1000

static void fputs(uint32_t file, const char* string)
{
	write(file, string, strlen(string));
}

int main(int argc, char** argv)
{
	uint32_t uart_file = open("DEV:uart/0", NFile_Open_Mode::Read_Write);

	TUART_IOCtl_Params params;
	params.baud_rate = NUART_Baud_Rate::BR_115200;
	params.char_length = NUART_Char_Length::Char_8;
	ioctl(uart_file, NIOCtl_Operation::Set_Params, &params);

    set_task_deadline(TASK_DEADLINE);

	fputs(uart_file, "UART test task starting!");

    char buf[16] = {0};
    int r = 0;
    for(;;)
    {
        while(wait(uart_file, 4, TASK_DEADLINE) == NSWI_Result_Code::Fail)
            ;
        r = read(uart_file, buf, 16);
        if (r > 0)
        {
            fputs(uart_file, "UART test task received: ");
            write(uart_file, buf, r);
            fputs(uart_file, "\r\n");
        }
        else
            fputs(uart_file, "UART test task received nothing\r\n");
        bzero(buf, 16);
    }
}
