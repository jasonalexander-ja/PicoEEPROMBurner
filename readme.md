## 28Cxxx EEPROM Burner for the Raspberry Pi Pico 

This is a simple program for burning 28Cxxx compatible 8 bit EEPROMs using a Raspberry Pi Pico,
to be used with this [client program](https://github.com/jasonalexander-ja/EEPROMBurnerClient). 

This works with EEPROMS from 28c16 to 28c256.

The circuit is as follows;

 - Pico GPIO 0-14 -> EEPROM Address A0-A14 (omit higher addresses if necessary)
 - Pico GPIO 15-22 -> EEPROM D0-D7

 - Pico GPIO 26 -> EEPROM Chip Select 
 - Pico GPIO 27 -> EEPROM Output Enable
 - Pico GPIO 28 -> EEPROM Write Enable

The serial communication baudrate is 115200, the device will 
act as a request response model, sending responses for commands.

The commands are as follows;

| **Command** | **Action**                                                                           | **Serial Response** |
|-------------|--------------------------------------------------------------------------------------|---------------------|
| LXXX        | Set the lower 8 bits of the address to XXX.                                          | A (acknowledge)     |
| UXXX        | Set the upper 7 bits of the address to XXX.                                          | A (acknowledge)     |
| DXXX        | Set the data pins to XXX.                                                            | A (acknowledge)     |
| F           | Toggles chip select and write enable low (F is for "flash").                         | A (acknowledge)     |
| R           | Toggle chip select and read enable low, reads and responds with value on data pins.  | DXXX (data)         |

<sub>Where XXX is an 8 bit unsigned integer in string decimal representation. </sub>

This is intent on being used with PlatformIO extension for VSCode for development and uploading. 
Commands can be sent just be typing them into a serial monitor, viewing the results, without the need of a client application. 
