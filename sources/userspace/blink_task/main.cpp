#include <stdstring.h>
#include <stdfile.h>
#include <stdmutex.h>

#include <drivers/gpio.h>
#include <process/process_manager.h>

int main(int argc, char** argv)
{
    volatile int i;

    uint32_t led_pin = open("DEV:gpio/47", NFile_Open_Mode::Write_Only);

    while (true)
    {
        write(led_pin, "1", 1);

        for (i = 0; i < 0x40000; i++)
            ;

        write(led_pin, "0", 1);

        for (i = 0; i < 0x40000; i++)
            ;
    }

    close(led_pin);
}