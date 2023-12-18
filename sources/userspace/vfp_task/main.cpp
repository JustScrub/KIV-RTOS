#include <stdfile.h>
#include <stdstring.h>

#include <process/process_manager.h>
#include <drivers/bridges/uart_defs.h>

const char *inps[] = {"3.0", "0.3", "-0.1", "-1.0", ".1", "1.", "-.2", "5", "-5",
					  "3.00", "0.30", "0.03", "-0.10", "-1.10", ".010", "1.0", "-.020", "5.0", "-5.0",
					  "24abc", "28.4ab", "-18\r\n", "-.018\r\n"};

void printfl(int fd,float b, char * buffer)
{

		int len = ftoa(b, buffer, 3);
		write(fd, buffer, len);
		write(fd, "\r\n", 2);
		bzero(buffer, len);
}

void printin(int fd, int b, char * buffer)
{

		int len = itoa(b, buffer, 10);
		write(fd, buffer, len);
		write(fd, "\r\n", 2);
		bzero(buffer, len);
}

int main(int argc, char** argv)
{

	int fd = open("DEV:uart/0", NFile_Open_Mode::Write_Only);
	
	TUART_IOCtl_Params params;
	params.baud_rate = NUART_Baud_Rate::BR_115200;
	params.char_length = NUART_Char_Length::Char_8;
	ioctl(fd, NIOCtl_Operation::Set_Params, &params);

	float a = 1.0f;
	float b = 2.8f;
	char buffer[20];
	bzero(buffer, 20);
	write(fd, "Hello world!\r\n", 14);
	printin(fd, (int)b, buffer);
	
	for (int i=0; i < sizeof(inps)/sizeof(char*); i++)
	{
		b = atof(inps[i]);
		printfl(fd, b, buffer);
		printin(fd, (int)b, buffer);
		write(fd, "----------------\r\n", 18);
	}
	b = atof("0.3");

	while (a < 100.0f)
	{
		a +=b;
		printfl(fd, a, buffer);
	}


}