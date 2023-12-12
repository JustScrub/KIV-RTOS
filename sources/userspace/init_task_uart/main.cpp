#include <stdfile.h>

#include <process/process_manager.h>


void blink(int sos_led,bool on)
{
	write(sos_led, on?"1": "0", 1);
	for(int i = 0; i < 100000; i++)
		;
}

int main(int argc, char** argv)
{
	// systemovy init task startuje jako prvni, a ma nejnizsi prioritu ze vsech - bude se tedy planovat v podstate jen tehdy,
	// kdy nic jineho nikdo nema na praci

	int led = open("DEV:gpio/47", NFile_Open_Mode::Write_Only);
	bool on = true;

	// nastavime deadline na "nekonecno" = vlastne snizime dynamickou prioritu na nejnizsi moznou
	set_task_deadline(Indefinite);
	
	while (true)
	{
		// kdyz je planovany jen tento proces, blikame ledkou
		if (get_active_process_count() == 1)
			blink(led, on);

		// predame zbytek casoveho kvanta dalsimu procesu
		sched_yield();
		on = !on;
	}
	
	return 0;
}