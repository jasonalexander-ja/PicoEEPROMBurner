#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include <Utils.h>

#define DATA_PINTS_MASK 0x7F800

#define UPPER_ADDRESS 0xFFFFF8FF
#define LOWER_ADDRESS 0xFFFFFF00
#define DATA 0xFFF807FF

#define CHIP_ENABLE 19
#define OUTPUT_ENABLE 20

#define WRITE_ENABLE 21

void loop();

void setup();

void process_command(char c);

void set_lower_addr();

void set_upper_addr();

void set_data();

void flash_data();

void read_data();


void set_data_dir(bool out)
{
	gpio_set_dir(11, out);  
	gpio_set_dir(12, out); 
	gpio_set_dir(13, out); 
	gpio_set_dir(14, out); 
	gpio_set_dir(15, out); 
	gpio_set_dir(16, out); 
	gpio_set_dir(17, out); 
	gpio_set_dir(18, out); 
}

int main(void)
{
	setup();

	while (true)
		loop();
}

void setup()
{
	stdio_init_all();

	gpio_init(WRITE_ENABLE);
	gpio_init(OUTPUT_ENABLE);
	gpio_init(CHIP_ENABLE);

	gpio_set_dir(WRITE_ENABLE, true);
	gpio_set_dir(OUTPUT_ENABLE, true);
	gpio_set_dir(CHIP_ENABLE, true);

	gpio_init(0);
	gpio_init(1);
	gpio_init(2);
	gpio_init(3);
	gpio_init(4);
	gpio_init(5);
	gpio_init(6);
	gpio_init(7);
	gpio_init(8);
	gpio_init(9);
	gpio_init(10);

	gpio_init(11);
	gpio_init(12);
	gpio_init(13);
	gpio_init(14);
	gpio_init(15);
	gpio_init(16);
	gpio_init(17);
	gpio_init(18);

	gpio_set_dir(0, true);
	gpio_set_dir(1, true);
	gpio_set_dir(2, true);
	gpio_set_dir(3, true);
	gpio_set_dir(4, true);
	gpio_set_dir(5, true);
	gpio_set_dir(6, true);
	gpio_set_dir(7, true);
	gpio_set_dir(8, true);
	gpio_set_dir(9, true);
	gpio_set_dir(10, true);

	gpio_put(WRITE_ENABLE, true);
	gpio_put(OUTPUT_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
}

void loop()
{
	char command[1] = {'0'};
	get_block(command, 1);

	process_command(command[0]);
}

void process_command(char c)
{
	switch (c)
	{
	case 'L':
		set_lower_addr();
		break;
	case 'U':
		set_upper_addr();
		break;
	case 'D':
		set_data();
		break;
	case 'F':
		flash_data();
		break;
	case 'R':
		read_data();
		break;
	}
}

void set_lower_addr()
{
	char address_str[3] = {'0', '0', '0'};
	get_block(address_str, 3);
	uint32_t address = atoi(address_str) & 0xFF;
	uint32_t curr_val = gpio_get_all();
	gpio_put_all((curr_val & LOWER_ADDRESS) | address);
}

void set_upper_addr()
{
	char address_str[3] = {'0', '0', '0'};
	get_block(address_str, 3);
	uint32_t address = (atoi(address_str) & 0b111) << 8;
	uint32_t curr_val = gpio_get_all();
	gpio_put_all((curr_val & UPPER_ADDRESS) | address);
}

void set_data()
{
	char data_str[3] = {'0', '0', '0'};
	get_block(data_str, 3);
	uint32_t data = (atoi(data_str) & 0xFF) << 11;
	uint32_t curr_val = gpio_get_all();
	set_data_dir(true);
	gpio_put_all((curr_val & DATA) | data);
}

void flash_data() 
{
	set_data_dir(true);

	gpio_put(CHIP_ENABLE, false);
	gpio_put(WRITE_ENABLE, false);
	sleep_ms(5);

	gpio_put(WRITE_ENABLE, true);
	gpio_put(CHIP_ENABLE, true);
}

void read_data() 
{
	set_data_dir(false);

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
