## 28C16 EEPROM Burner for the Raspberry Pi Pico 

This is a simple program for burning 28C16 compatible EEPROMs using a Raspberry Pi Pico,
to be used with this [client program](https://github.com/jasonalexander-ja/EEPROMBurnerClient). 

The circuit is as follows;

 - Pico GPIO 0-10 -> EEPROM Address A0-A10
 - Pico GPIO 11-18 -> EEPROM I/O 0-I/O 7

 - Pico GPIO 19 -> EEPROM Chip Select 
 - Pico GPIO 20 -> EEPROM Output Enable
 - Pico GPIO 21 -> EEPROM Write Enable

The serial communication baudrate is 115200, the device will 
act as a request response model, sending responses for commands.

The commands are as follows;

| **Command** | **Response**                                                                         | **Respnse**     |
|-------------|--------------------------------------------------------------------------------------|-----------------|
| LXXX        | Set the lower 8 bits of the address to XXX.                                          | A (acknowledge) |
| UXXX        | Set the upper 3 bits of the address to XXX.                                          | A (acknowledge) |
| DXXX        | Set the data pins to XXX.                                                            | A (acknowledge) |
| F           | Toggles chip select and write enable low (F is for "flash").                         | A (acknowledge) |
| R           | Toggle chip select and read enable low, reads and responds with value on data pins.  | DXXX (data)     |

<sub>Where XXX is an 8 bit unsigned integer in string representation. </sub>

This is intent on being used with PlatformIO extension for VSCode for development and uploading. 
