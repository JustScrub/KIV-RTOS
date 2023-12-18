#pragma once
#include "glukose_pred.cpp"
#include <stdstring.h>
#include <stdfile.h>
#include <drivers/bridges/uart_defs.h>

int uart_fd = -1;
void printf(const char* string)
{
    write(uart_fd, string, strlen(string));
    write(uart_fd, "\r\n", 2);
}

void print_params(params *p)
{
    char bfr[32];
    write(uart_fd, "A=", 2);
    ftoa(p->A, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, "; B=", 4);
    ftoa(p->B, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, "; C=", 4);
    ftoa(p->C, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, "; D=", 4);
    ftoa(p->D, bfr, 4);
    write(uart_fd, bfr, strlen(bfr));

    write(uart_fd, "; E=", 4);
    ftoa(p->E, bfr, 4);
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
