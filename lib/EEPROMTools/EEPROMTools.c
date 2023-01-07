#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <Utils.h>

#define DATA_PINTS_MASK 0x7F800
#define ADDRESS_PINS_MASK 0x7FF

#define LADDRESS_PINS_MASK 0xFF
#define UADDRESS_PINS_MASK 0x700

#define UPPER_ADDRESS 0xFFFFF8FF
#define LOWER_ADDRESS 0xFFFFFF00
#define DATA 0xFFF807FF

#define CHIP_ENABLE 19
#define OUTPUT_ENABLE 20
#define WRITE_ENABLE 21

#define CONTROL_BITS_MASK 0x380000

void setup_gpio()
{
	stdio_init_all();

	gpio_init_mask(ADDRESS_PINS_MASK | CONTROL_BITS_MASK | DATA_PINTS_MASK);

	gpio_set_dir_out_masked(ADDRESS_PINS_MASK | CONTROL_BITS_MASK);

	gpio_put_masked(CONTROL_BITS_MASK, CONTROL_BITS_MASK);
}

void set_lower_addr()
{
	char address_str[3] = {'0', '0', '0'};
	get_block(address_str, 3);

	uint32_t address = atoi(address_str) & 0xFF;
	
	gpio_put_masked(LADDRESS_PINS_MASK, address);
}

void set_upper_addr()
{
	char address_str[3] = {'0', '0', '0'};
	get_block(address_str, 3);

	uint32_t address = (atoi(address_str) & 0b111) << 8;
	
	gpio_put_masked(UADDRESS_PINS_MASK, address);
}

void set_data()
{
	char data_str[3] = {'0', '0', '0'};
	get_block(data_str, 3);

	uint32_t data = (atoi(data_str) & 0xFF) << 11;

	gpio_set_dir_out_masked(DATA_PINTS_MASK);
	gpio_put_masked(DATA_PINTS_MASK, data);
}

void flash_data() 
{
	gpio_set_dir_out_masked(DATA_PINTS_MASK);

	gpio_put(CHIP_ENABLE, false);
	gpio_put(WRITE_ENABLE, false);
	sleep_ms(5);

	gpio_put(WRITE_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
}

void read_data() 
{
	gpio_set_dir_in_masked(DATA_PINTS_MASK);

	gpio_put(CHIP_ENABLE, false);
	gpio_put(OUTPUT_ENABLE, false);
	sleep_ms(5);

	int curr_val = (gpio_get_all() & DATA_PINTS_MASK) >> 11;
	printf("D%i\n", curr_val);
	sleep_ms(5);

	gpio_put(OUTPUT_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
	sleep_ms(5);
}

void process_command(char c)
{
	switch (c)
	{
		case 'L': set_lower_addr(); break;
		case 'U': set_upper_addr(); break;
		case 'D': set_data(); break;
		case 'F': flash_data(); break;
		case 'R': read_data(); 	return;
		default:
			return;
	}
	printf("A\n");
}

void process_command_loop()
{
	char command[1] = {'0'};
	get_block(command, 1);

	process_command(command[0]);
}
