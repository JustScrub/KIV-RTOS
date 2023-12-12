#include <stdfile.h>

#include <process/process_manager.h>

int main(int argc, char** argv)
{
	// systemovy init task startuje jako prvni, a ma nejnizsi prioritu ze vsech - bude se tedy planovat v podstate jen tehdy,
	// kdy nic jineho nikdo nema na praci

	// nastavime deadline na "nekonecno" = vlastne snizime dynamickou prioritu na nejnizsi moznou
	set_task_deadline(Indefinite);
	int led = open("DEV:gpio/47", NFile_Open_Mode::Write_Only);
	bool on = false;
	// TODO: tady budeme chtit nechat spoustet zbytek procesu, az budeme umet nacitat treba z eMMC a SD karty
	
	while (true)
	{
		// kdyz je planovany jen tento proces, pockame na udalost (preruseni, ...)
		if (get_active_process_count() == 1)
		{
			//asm volatile("wfe");
			write(led, on ? "1" : "0", 1);
			on = !on;
			for(int i = 0; i < 0x800000; i++);
		}
		// predame zbytek casoveho kvanta dalsimu procesu
			write(led, on ? "1" : "0", 1);
			on = !on;
			for(int i = 0; i < 0x40000; i++);
		sched_yield();
	}
	
	return 0;
}