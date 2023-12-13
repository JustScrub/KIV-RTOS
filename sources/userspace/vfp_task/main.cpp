#include <stdfile.h>
#include <stdstring.h>

#include <process/process_manager.h>

int main(int argc, char** argv)
{

	int fd = open("/dev/uart0", NFile_Open_Mode::Write_Only);
	float a = 1.f;
	float b = atof("0.3");
	char buffer[20];

	while (true)
	{
		a +=b;
		int len = ftoa(a, buffer, 3);
		write(fd, buffer, len);
	}


}