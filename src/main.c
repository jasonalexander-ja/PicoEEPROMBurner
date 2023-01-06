#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <EEPROMTools.h>

int main(void)
{
	setup_gpio();

	while (true) process_command_loop();
}
