#pragma once

#include <hal/peripherals.h>
#include <drivers/bcm_aux.h>
#include <drivers/bridges/uart_defs.h>
#include <fs/filesystem.h>

constexpr int UART_BFR_SIZE=256;
constexpr int UART_IRQ_WAIT=0x80000;

class CUART
{
    friend class CUART_File;
    friend class CUART_FS_Driver;
    private:
        // odkaz na AUX driver
        CAUX& mAUX;

        // byl UART kanal otevreny?
        bool mOpened;

        // vlastnik tohoto UART kanalu
        IFile* mOwner;

        // nastavena baud rate, ukladame ji proto, ze do registru se uklada (potencialne ztratovy) prepocet
        NUART_Baud_Rate mBaud_Rate;

        char mBuffer[UART_BFR_SIZE];
        unsigned int mBuffer_Tail = 0;
        unsigned int mBuffer_Count = 0;

    public:
        CUART(CAUX& aux);

        // otevre UART kanal, exkluzivne
        bool Open();
        // uzavre UART kanal, uvolni ho pro ostatni
        void Close();
        // je UART kanal momentalne otevreny?
        bool Is_Opened() const;

        NUART_Char_Length Get_Char_Length();
        void Set_Char_Length(NUART_Char_Length len);

        NUART_Baud_Rate Get_Baud_Rate();
        void Set_Baud_Rate(NUART_Baud_Rate rate);

        // miniUART na RPi0 nepodporuje nic moc jineho uzitecneho, napr. paritni bity, vice stop-bitu nez 1, atd.

        void Write(char c);
        void Write(const char* str);
        void Write(const char* str, unsigned int len);
        void Write(unsigned int num);
        void Write_Hex(unsigned int num);

        // TODO: read (budeme to pak nejspis propojovat s prerusenim)
        uint32_t Read(char *buf, unsigned int len);
        uint32_t Get_Bytes_Available();
        bool Is_IRQ_Pending();
        void IRQ_Callback();
};

extern CUART sUART0;
