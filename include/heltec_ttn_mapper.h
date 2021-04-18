#include "Arduino.h"
#include <ESP32_LoRaWAN.h>
#include <TinyGPS++.h>

// Display is initializied in LoraWan -- SSD1306Wire - just use it.
#define GPS_RX 23
#define GPS_TX 22

/*license for Heltec ESP32 LoRaWan, quary your ChipID relevant license: http://resource.heltec.cn/search */
//uint32_t license[4] = {0xFA009A56, 0x5E5FC459, 0xE47E4407, 0xD81E58D2};
uint32_t license[4] = {0x000000, 0x0, 0x0, 0x0};

/* OTAA para*/
uint8_t DevEui[] = {0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t AppEui[] = {0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t AppKey[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/* ABP para*/
uint8_t NwkSKey[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t AppSKey[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint32_t DevAddr = (uint32_t)0x0;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 45000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = false;

/* Application port */
uint8_t appPort = 2;

/* counter of search...*/
int search_counter = 0;

/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 7;

/*LoraWan debug level, select in arduino IDE tools.
* None : print basic info.
* Freq : print Tx and Rx freq, DR info.
* Freq && DIO : print Tx and Rx freq, DR, DIO0 interrupt and DIO1 interrupt info.
* Freq && DIO && PW: print Tx and Rx freq, DR, DIO0 interrupt, DIO1 interrupt, MCU sleep and MCU wake info.


build_flags = 
	-D REGION_EU868
	-D ACTIVE_REGION=LORAMAC_REGION_EU868
	-D LoRaWAN_DEBUG_LEVEL=0
*/
uint8_t debugLevel = LoRaWAN_DEBUG_LEVEL;

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_EU868;

// https://stackoverflow.com/questions/21005845/how-to-get-float-in-bytes#21005927
// The TinyGPS++ object
TinyGPSPlus gps;
typedef union
{
  float f[4];              // Assigning fVal.f will also populate fVal.bytes;
  unsigned char bytes[16]; // Both fVal.f and fVal.bytes share the same 4 bytes of memory.
} floatArr2Val;
floatArr2Val latlong;
float latitude;
float longitude;
float altitude;
float hdop;
char s[16]; // used to sprintf for OLED display

