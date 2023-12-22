#pragma once
#include "glukose_pred.cpp"
#include <stdstring.h>
#include <stdfile.h>
#include <drivers/bridges/uart_defs.h>

int uart_fd = -1;

bool open_uart(NUART_Baud_Rate rate = NUART_Baud_Rate::BR_115200, NUART_Char_Length char_len = NUART_Char_Length::Char_8)
{
    uart_fd = open("DEV:uart/0", NFile_Open_Mode::Read_Write);

    TUART_IOCtl_Params params;
    params.baud_rate = rate;
    params.char_length = char_len;
    params.ISR_wait_cntr = 0xC0000;
    ioctl(uart_fd, NIOCtl_Operation::Set_Params, &params);

    return uart_fd >= 0;
}

void printf(const char* string, bool nl=true)
{
    write(uart_fd, string, strlen(string));
    if(nl)
        write(uart_fd, "\r\n", 2);
}

void printfloat(float b, int afterpoint=3)
{
    char buffer[20];
    int len = ftoa(b, buffer, afterpoint);
    write(uart_fd, buffer, len);
    write(uart_fd, "\r\n", 2);
}

void printint(int b)
{
    char buffer[20];
    int len = itoa(b, buffer,10);
    write(uart_fd, buffer, len);
    write(uart_fd, "\r\n", 2);
}

void print_params(params *p)
{
    char bfr[32];
    write(uart_fd, "A=", 2);
    ftoa(p->A, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, ", B=", 4);
    ftoa(p->B, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, ", C=", 4);
    ftoa(p->C, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, ", D=", 4);
    ftoa(p->D, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, ", E=", 4);
    #if !GLPRED_LINREG
        ftoa(1.0f/p->E, bfr, 4);
    #else
        ftoa(p->E, bfr, 4);
    #endif
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, "\r\n", 2);
}

// non-blocking read
int scanf(char *bfr, uint32_t min_size=1)
{
    int sz = 0, i;
    while( (i = read(uart_fd, bfr+sz, min_size)) )
    {
        sz += i;
    }
    bfr[sz] = '\0';
    return sz;
}

// blocking read
int bscanf(char *bfr, uint32_t min_size=1)
{
    while(wait(uart_fd,min_size)==NSWI_Result_Code::Fail) ;
    return scanf(bfr, min_size);
}

void get_numeric_input(char *bfr, uint32_t bfr_size)
{
    while(1)
    {
        bzero(bfr, bfr_size);
        bscanf(bfr);

        if(is_numeric(bfr))
            return;
        printf("invalid input -- must be number\n");
    }
}