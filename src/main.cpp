// Adapted on Heltec original Lora-Library and tinyGPS+ Exampel.
// https://www.thethingsnetwork.org/labs/story/heltec-lorawan-gps-quick-start
// GPS get_coords function adapted from here: https://github.com/brady-aiello/Seeeduino_LoRaWAN_for_hybrid_gateways

/* Decode coordinate payload for TTN console
function Bytes2Float32(bytes) {
    var sign = (bytes & 0x80000000) ? -1 : 1;
    var exponent = ((bytes >> 23) & 0xFF) - 127;
    var significand = (bytes & ~(-1 << 23));
    if (exponent == 128)
        return sign * ((significand) ? Number.NaN : Number.POSITIVE_INFINITY);
    if (exponent == -127) {
        if (significand === 0) return sign * 0.0;
        exponent = -126;
        significand /= (1 << 22);
    } else significand = (significand | (1 << 23)) / (1 << 23);
    return sign * significand * Math.pow(2, exponent);
  }
  function Decoder(bytes, port) {
  var lat = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
  var lon = bytes[7] << 24 | bytes[6] << 16 | bytes[5] << 8 | bytes[4];
  var alt = bytes[11] << 24 | bytes[10] << 16 | bytes[9] << 8 | bytes[8];
  var hdop = bytes[15] << 24 | bytes[14] << 16 | bytes[13] << 8 | bytes[12];
  return{
    latitude:  Bytes2Float32(lat),
    longitude: Bytes2Float32(lon),
    altitude: Bytes2Float32(alt),
    hdop: Bytes2Float32(hdop),
  };
  }
*/
#include <pins_arduino.h>
#include <ESP32_LoRaWAN.h>
#include <TinyGPS++.h>
#include "heltec_ttn_mapper.h" //define your Variables HERE

bool validate()
{
  if ((latitude == 0) | (longitude == 0) | (hdop == 0))
  {
    return false;
  }
  else
  {
    return true;
  }
}

void wakeup_display()
{
  Display.init();
  delay(20);
  Display.wakeup();
}

/*Please don't !!! use Debug-Code (Serial.print) within coding - map all VIEW-Code into own functions - so that we at least
TRY to implement Model-View-Controller Pattern. */
void display_data_8x8()
{
  wakeup_display();
  int textsize = 10;
  Display.clear();
  Display.setFont(ArialMT_Plain_10);
  Display.setTextAlignment(TEXT_ALIGN_LEFT);

  if (validate())
  {

    sprintf(s, "Lat:  %f", latitude);
    Display.drawString(10, textsize * 0, s); 

    sprintf(s, "Lng: %f", longitude);
    Display.drawString(10, textsize * 1, s);

    sprintf(s, "Alt: %f", altitude);
    Display.drawString(10, textsize * 2, s);

    sprintf(s, "hdop: %f", hdop);
    Display.drawString(10, textsize * 3, s);

    switch (deviceState)
    {
    case DEVICE_STATE_INIT:
    {
      sprintf(s, "Lorawan: INIT");
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      sprintf(s, "Lorawan: JOIN");
      break;
    }
    case DEVICE_STATE_SEND:
    {
      sprintf(s, "Lorawan: SEND");
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      sprintf(s, "Lorawan: CYCLE");
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      sprintf(s, "Lorawan: SLEEP");
      break;
    }
    default:
    {
      sprintf(s, "Lorawan: Error-State");
      break;
    }
    }

    Display.drawString(10, textsize * 4, s);

    search_counter = 0; // we had a match.
  }
  else
  {
    search_counter++;
    Display.drawString(10, textsize * 0, "Search for GPS Signal...");
    sprintf(s, "Searched %d loops", search_counter);
    Display.drawString(10, textsize * 1, s);
  }
  Display.display();
}

void display_data_serial()
{
  sprintf(s, "%f", latitude);
  Serial.print("lat: ");
  Serial.println(s);

  sprintf(s, "%f", longitude);
  Serial.print("long: ");
  Serial.println(s);

  sprintf(s, "%f", altitude);
  Serial.print("alt: ");
  Serial.println(s);

  sprintf(s, "%f", hdop);
  Serial.print("hdop: ");
  Serial.println(s);

  Serial.print("LORAWAN Hex:");
  for (int i = 0; i < sizeof(latlong.bytes); i++)
  {
    Serial.print(latlong.bytes[i], HEX);
  }
  Serial.println(":");
}

void get_coords()
{
  //  Serial.println("read GPS... ");
  while (Serial2.available())
    gps.encode(Serial2.read());
  latitude = gps.location.lat();
  longitude = gps.location.lng();
  altitude = gps.altitude.meters();
  if (!validate())
  {
    delay(5000);
  } // Wait for Satelitte link
  hdop = gps.hdop.hdop();
  // Only update if location is valid and has changed
  if ((latitude && longitude) && latitude != latlong.f[0] && longitude != latlong.f[1])
  {
    latlong.f[0] = latitude;
    latlong.f[1] = longitude;
    latlong.f[2] = altitude;
    latlong.f[3] = hdop;
  }
}

static void prepareTxFrame(uint8_t port)
{
  get_coords();
  appDataSize = sizeof(latlong.bytes); //AppDataSize max value is 64
  for (int i = 0; i < appDataSize; i++)
  {
    appData[i] = latlong.bytes[i];
  }
}

// Add your initialization code here
void setup()
{
  Serial2.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  while (!Serial2)
  {
    Serial.println("wait for Serial2...");
    delay(1000);
  }

  Serial.begin(115200);
  while (!Serial)
    ;

  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);
  deviceState = DEVICE_STATE_INIT;
  if (mcuStarted == 0)
  {
    wakeup_display();
  }
}

// The loop function is called in an endless loop
void loop()
{
  if (validate()) // check for not 0 Values - simple but functional
  {
    switch (deviceState)
    {
    case DEVICE_STATE_INIT:
    {
      LoRaWAN.init(loraWanClass, loraWanRegion);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.displayJoining();
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      // LoRaWAN.displaySending();
      prepareTxFrame(appPort);
      display_data_serial();
      display_data_8x8();
      LoRaWAN.send(loraWanClass);
      delay(15000); // show data a while in the display
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      // Serial.print("Cycle");
      txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      //   LoRaWAN.displayAck();
      LoRaWAN.sleep(loraWanClass, debugLevel);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
    }
  }
  else
  {
    get_coords(); // read coords - wait for GPS catch - please be aware that inhouse coordinates may not ever leave this part of the programm
    display_data_serial();
    display_data_8x8();
    delay(5000);
  }
}