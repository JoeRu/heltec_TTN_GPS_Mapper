#include "Arduino.h"
#include <ESP32_LoRaWAN.h>
#include <TinyGPS++.h>

//#define GPS_RX 23
//#define GPS_TX 22

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 55000;

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
  float f[5];              // Assigning fVal.f will also populate fVal.bytes;
  unsigned char bytes[20]; // 6*4 bytes for float = Both fVal.f and fVal.bytes share the same 4 bytes of memory.
} floatArr2Val;
floatArr2Val latlong;
float latitude;  //floatArr2Val[0]
float longitude; //floatArr2Val[1]
float altitude;  //floatArr2Val[2]
float hdop;      //floatArr2Val[3]
float vbatt;     //floatArr2Val[4]
float vin;       //

char s[16]; // used to sprintf for OLED display

#define Fbattery 5540 //The default battery is 3700mv when the battery is fully charged.

#define XS 1.0 //The returned reading is multiplied by this XS to get the battery voltage.


