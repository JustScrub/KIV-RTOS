#include <stdfile.h>
#define WFE_BLINK

int main(int argc, char** argv)
{
	// systemovy init task startuje jako prvni, a ma nejnizsi prioritu ze vsech - bude se tedy planovat v podstate jen tehdy,
	// kdy nic jineho nikdo nema na praci

	// nastavime deadline na "nekonecno" = vlastne snizime dynamickou prioritu na nejnizsi moznou
	set_task_deadline(Indefinite);
	// TODO: tady budeme chtit nechat spoustet zbytek procesu, az budeme umet nacitat treba z eMMC a SD karty
	
	#ifdef WFE_BLINK
		int led_fd = open("DEV:gpio/47", NFile_Open_Mode::Write_Only);
	#endif

	while (true)
	{
		// kdyz je planovany jen tento proces, pockame na udalost (preruseni, ...)
		if (get_active_process_count() == 1)
		{
			#ifdef WFE_BLINK
				write(led_fd, "0", 1);
			#endif

			asm volatile("wfe");

			#ifdef WFE_BLINK
				write(led_fd, "1", 1);
			#endif
		}
		// predame zbytek casoveho kvanta dalsimu procesu
		sched_yield();
	}
	
	return 0;
}