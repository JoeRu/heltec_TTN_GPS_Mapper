# heltec_TTN_GPS_Mapper

This project aims to build a thethingsnetwork GSP Mapper - ttnmapper.org. It makes use of PlattformIO https://docs.platformio.org/ as toolkit - because of the library management and the much better IDE - and the unit-testing cappabilities; 

Running it with Arduino should be possible - but not out of the box - and require some adoptions.

It makes use of a https://heltec.org/project/wifi-lora-32/ board and a GPS Module (in my case a NEO-6M GPS Modul).

The project makes use of the Heltec-Original LoraWAN Library https://github.com/HelTecAutomation/ESP32_LoRaWAN
and TinyGPSPlus https://github.com/mikalhart/TinyGPSPlus.

For reading Serial from RX/TX the Pins 22 and 23 are used in default to the UART Hardware-Processor on the board.
Serial2:
#define GPS_RX 23
#define GPS_TX 22

@TODO after cloning:
Please adapt all TTN-related Keys and variables in heltec_ttn_mapper.h
Please adapt your USB-Ports and your Lora-Frequencies in platfromio.ini.