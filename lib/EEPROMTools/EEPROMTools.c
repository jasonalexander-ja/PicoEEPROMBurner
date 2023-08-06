#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <Utils.h>

// GPIO Pins 15-22 are data pins 0-7 
#define DATA_PINTS_MASK 0x7F8000
// GPIO Pins 0-14 are address pins 0-14 
#define ADDRESS_PINS_MASK 0x7FFF

// Address pins are divided into the lower 8 bits and upper 7 bits 
// GPIO Pins 0-7 are address pins 0-7 (L for Lower Address) 
#define LADDRESS_PINS_MASK 0xFF
// GPIO Pins 8-14 are address pins 8-14 (U for Upper Address) 
#define UADDRESS_PINS_MASK 0x7F00

// Skip pins 23, 24, and 25 as these are pads on the bottom

// Pins 26-28 are the control pins for R/W operations to the chip 
#define CONTROL_BITS_MASK 0x1C000000
#define CHIP_ENABLE 26
#define OUTPUT_ENABLE 27
#define WRITE_ENABLE 28

// Called on setup, sets the GPIO direction and all the control pins high (logical 0) 
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

	uint32_t address = (atoi(address_str) & 0x7F) << 8;
	
	gpio_put_masked(UADDRESS_PINS_MASK, address);
}

void set_data(int in_data)
{
	uint32_t data = (in_data & 0xFF) << 15;

	gpio_set_dir_out_masked(DATA_PINTS_MASK);
	gpio_put_masked(DATA_PINTS_MASK, data);
}

void fetch_set_data()
{
	char data_str[3] = {'0', '0', '0'};
	get_block(data_str, 3);

	uint32_t data = atoi(data_str);

	set_data(data);
}

void flash_data() 
{
	gpio_set_dir_out_masked(DATA_PINTS_MASK);

	gpio_put(CHIP_ENABLE, false);
	gpio_put(WRITE_ENABLE, false);
	sleep_us(150);

	gpio_put(WRITE_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
}

uint32_t read_data() 
{
	gpio_set_dir_in_masked(DATA_PINTS_MASK);

	gpio_put(CHIP_ENABLE, false);
	gpio_put(OUTPUT_ENABLE, false);
	sleep_us(50);

	uint32_t curr_val = (gpio_get_all() & DATA_PINTS_MASK) >> 15;
	sleep_us(50);

	gpio_put(OUTPUT_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
	sleep_us(50);
	return curr_val;
}

void read_data_print()
{
	uint32_t curr_val = read_data();
	printf("D%lu\n", curr_val);
}

void set_data_flash()
{
	char data_str[3] = {'0', '0', '0'};
	get_block(data_str, 3);

	uint32_t data = atoi(data_str);
	uint32_t readback_data = read_data();

	while (data != readback_data)
	{
		set_data(data);
		flash_data();
		sleep_us(400);
		readback_data = read_data();
	}

}

void process_command(char c)
{
	switch (c)
	{
		case 'L': set_lower_addr(); break;
		case 'U': set_upper_addr(); break;
		case 'D': fetch_set_data(); break;
		case 'F': flash_data(); break;
		case 'S': set_data_flash(); break;
		case 'R': read_data_print(); 	return;
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
