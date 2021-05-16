// Adapted on Heltec original Lora-Library and tinyGPS+ Exampel.
// https://www.thethingsnetwork.org/labs/story/heltec-lorawan-gps-quick-start
// GPS get_coords function adapted from here: https://github.com/brady-aiello/Seeeduino_LoRaWAN_for_hybrid_gateways

/*
/* Decode coordinate payload for TTN console V3 * /
// COPY HERE
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
  
  function Decoder(bytes) {
  var lat = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
  var lon = bytes[7] << 24 | bytes[6] << 16 | bytes[5] << 8 | bytes[4];
  var alt = bytes[11] << 24 | bytes[10] << 16 | bytes[9] << 8 | bytes[8];
  var hdop = bytes[15] << 24 | bytes[14] << 16 | bytes[13] << 8 | bytes[12];
  var vbatt = bytes[19] << 24 | bytes[18] << 16 | bytes[17] << 8 | bytes[16];
  return{
    latitude:  Bytes2Float32(lat),
    longitude: Bytes2Float32(lon),
    altitude: Bytes2Float32(alt),
    hdop: Bytes2Float32(hdop),
    vbat: Bytes2Float32(vbatt),
  };
  }
  
function decodeUplink(input) {
  return {
    data: Decoder(input.bytes)
  };
}
// STOP COPY CODE FOR TTN

/* DEBUG Code for NODEJS - 

let payload = {"end_device_ids":{"device_id":"jrsht1-ttnmapper","application_ids":{"application_id":"jrs-gpstracker"},"dev_eui":"0026F74B62A6F24F","dev_addr":"260BE164"},"correlation_ids":["as:up:01F5TQ586HM8SVBN0D9EBCRFQJ","gs:conn:01F5T9G3R6Y7GC4GSHKDMPWQ6T","gs:up:host:01F5T9G3RE5DS9KJAMDJY1TJG7","gs:uplink:01F5TQ57ZVGF6KBBQ1T3SPHP8X","ns:uplink:01F5TQ57ZWTBNGKAQN58SP7K16","rpc:/ttn.lorawan.v3.GsNs/HandleUplink:01F5TQ57ZW25FSWE9AHXCWPK1B","rpc:/ttn.lorawan.v3.NsAs/HandleUplink:01F5TQ586GJ3CTT5C9BWSSTSDJ"],"received_at":"2021-05-16T13:51:57.138507721Z","uplink_message":{"f_port":2,"f_cnt":155,"frm_payload":"Y09JQumgCEHNDJBDMzMzP5j2SEA=","decoded_payload":{"bytes":[99,79,73,66,233,160,8,65,205,12,144,67,51,51,51,63,152,246,72,64]},"rx_metadata":[{"gateway_ids":{"gateway_id":"jayars-usi1-mikrotik","eui":"343632383A001D00"},"time":"2021-05-16T13:51:55.746319Z","timestamp":4145206710,"rssi":-85,"channel_rssi":-85,"snr":10.25,"location":{"latitude":50.32750159,"longitude":8.53941504,"altitude":294,"source":"SOURCE_REGISTRY"},"uplink_token":"CiIKIAoUamF5YXJzLXVzaTEtbWlrcm90aWsSCDQ2Mjg6AB0AELary7gPGgwI/MaEhQYQh8CzuAMg8J3LjNLvAw=="},{"gateway_ids":{"gateway_id":"packetbroker"},"packet_broker":{"message_id":"01F5TQ57ZYJX1APEYQG00G4PYT","forwarder_net_id":"000013","forwarder_tenant_id":"ttn","forwarder_cluster_id":"ttn-v2-eu-4","home_network_net_id":"000013","home_network_tenant_id":"ttn","home_network_cluster_id":"ttn-eu1","hops":[{"received_at":"2021-05-16T13:51:56.926248543Z","sender_address":"52.169.150.138","receiver_name":"router-dataplane-57d9d9bddd-xjszp","receiver_agent":"pbdataplane/1.5.2 go/1.16.2 linux/amd64"},{"received_at":"2021-05-16T13:51:56.933927868Z","sender_name":"router-dataplane-57d9d9bddd-xjszp","sender_address":"forwarder_uplink","receiver_name":"router-5b5dc54cf7-psxlt","receiver_agent":"pbrouter/1.5.2 go/1.16.2 linux/amd64"},{"received_at":"2021-05-16T13:51:56.948860132Z","sender_name":"router-5b5dc54cf7-psxlt","sender_address":"deliver.000013_ttn_ttn-eu1.uplink","receiver_name":"router-dataplane-57d9d9bddd-dsrjj","receiver_agent":"pbdataplane/1.5.2 go/1.16.2 linux/amd64"}]},"time":"2021-05-16T13:51:55.746319Z","rssi":-85,"channel_rssi":-85,"snr":10.25,"uplink_token":"eyJnIjoiWlhsS2FHSkhZMmxQYVVwQ1RWUkpORkl3VGs1VE1XTnBURU5LYkdKdFRXbFBhVXBDVFZSSk5GSXdUazVKYVhkcFlWaFphVTlwU1hsVlZFWnRXREpHYm1GR2FGZE5SWEF3WkRCR2JrbHBkMmxrUjBadVNXcHZhVm93Vmt4aFYzUkxZMnMxZEZSRlVUTmtSVEZ6WkhwV1RGVldUbUZhZVVvNUxtVmFNbGhOVTFKdmIxbFZialZMZERCWE56Z3pXbWN1TVRsemFEQkhjRlp4Tm1SUU1UUnJOaTQzYUV0aU0xUkZlVUp5TlVoVlRTMWtORzAyYjJkTGVWbzJTbTFoU2xreldEQndaalpNYTA1SloxUXdka2xXZVRocVFtVndUbTlyT0RWS2EweHlaa1ZKTW10VFpYaFNhbW8wVWxBMmVtdG9aWE16Vkd0SU4xRlZORTFqT0VGRk1taElWbUpOVjAxMk1XWmlXVmhpY2kxa2VFMXNOV293U0dOcVQzVm1WMmM0ZFhoRVZsVlBjbXd5U1UwME9YbGtjM0JvZEZnNVdtVm1jVlUxU21OMVJGVllUR3A2TTE5ak5rVm5iVkIwTGt4elpYTXpOMnR1VEdoblQzaDJUWEp3Y1RFekxYYz0iLCJhIjp7ImZuaWQiOiIwMDAwMTMiLCJmdGlkIjoidHRuIiwiZmNpZCI6InR0bi12Mi1ldS00In19"}],"settings":{"data_rate":{"lora":{"bandwidth":125000,"spreading_factor":12}},"coding_rate":"4/5","frequency":"868100000","timestamp":4145206710,"time":"2021-05-16T13:51:55.746319Z"},"received_at":"2021-05-16T13:51:56.924584996Z","consumed_airtime":"1.810432s"}};
var input = {"bytes": payload.uplink_message.decoded_payload.bytes, "port": 2};

console.log(JSON.stringify(decodeUplink(input)));


frm_payload = "Xk9JQumgCEEAAAAAAAAAAJj2SEA=";
let buff = Buffer.from(frm_payload, 'base64');  
input = {"bytes": buff, "port": 2};
console.log(JSON.stringify(decodeUplink(input)));
* /

*/
#include <pins_arduino.h>
#include <ESP32_LoRaWAN.h>
#include <TinyGPS++.h>
#include "heltec_ttn_mapper.h" 
//#include "ttn_variables.h" //define your Variables HERE or copy file to a device-specific include - like this:
//#include "jrs-heltec-gps2.h" //ESP32ChipID=4076F....
#include "jrs-heltec1_ABP.h" //ESP32ChipID=DC6CFC....


#define WITH_DEBUG_OUT //comment out - to disable serial-debug-output

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
  Display.flipScreenVertically();
}

void display_data_8x8()
{
  wakeup_display();
  int textsize = 10;
  Display.clear();
  Display.setFont(ArialMT_Plain_10);
  Display.setTextAlignment(TEXT_ALIGN_LEFT);
  int line = 0;

  sprintf(s, "Vbat: %.02f V", vbatt);
  Display.drawString(10, textsize * line, s);
  line++;

  if (validate())
  {
    sprintf(s, "Lat:  %f", latitude);
    Display.drawString(10, textsize * line, s);
    line++;

    sprintf(s, "Lng: %f", longitude);
    Display.drawString(10, textsize * line, s);
    line++;

    sprintf(s, "Alt: %f", altitude);
    Display.drawString(10, textsize * line, s);
    line++;

    sprintf(s, "hdop: %f", hdop);
    Display.drawString(10, textsize * line, s);
    line++;

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

    Display.drawString(10, textsize * line, s);
    line++;

    search_counter = 0;
  }
  else
  {
    search_counter++;
    Display.drawString(10, textsize * line, "Search for GPS Signal...");
    line++;
    
    sprintf(s, "hdop: %f", hdop);
    Display.drawString(10, textsize * line, s);
    line++;

    sprintf(s, "gpsbytes: %d", gps.charsProcessed());
    Display.drawString(10, textsize * line, s);
    line++;
  }

  Display.display();
}

#if defined(WITH_DEBUG_OUT)
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

  sprintf(s, "Vbat: %.02f V", vbatt);
  Serial.println(s);

  sprintf(s, "gpsbytes: %d", gps.charsProcessed());
  Serial.println(s);
}
#endif

double ReadVoltage(byte pin)
{                                   // https://github.com/HelTecAutomation/Heltec_ESP32/blob/62228d5a64dbe88989285519b4077be7b4125ce5/examples/ESP32/ADC_Read_Voltage/ADC_Read_Accurate/ADC_Read_Accurate.ino
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if (reading < 1 || reading >= 4095)
    return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required

void get_coords()
{
#if defined(WITH_DEBUG_OUT)
  Serial.println("read GPS... ");
#endif
  while (Serial2.available())
    gps.encode(Serial2.read());

  latitude = gps.location.lat();
  longitude = gps.location.lng();
  altitude = gps.altitude.meters();
  hdop = gps.hdop.hdop();

  // Only update if location is valid and has changed
  if ((latitude && longitude) && latitude != latlong.f[0] && longitude != latlong.f[1])
  {
    latlong.f[0] = latitude;
    latlong.f[1] = longitude;
    latlong.f[2] = altitude;
    latlong.f[3] = hdop;
  }

  //Vbat + Vin detection -- Not WIFI Compatible
  //Battery voltage read pin changed from GPIO13 to GPI37
  //Battery voltage read pin changed from GPIO13 to GPI37
  vbatt = ReadVoltage(37);

  latlong.f[4] = vbatt;
  delay(100);
  vin = analogRead(36) * 0.769 + 150;
  // latlong.f[5] = vin;
}

static void prepareTxFrame(uint8_t port)
{
  get_coords();
  appDataSize = sizeof(latlong.bytes); //AppDataSize max value is 64
  for (int i = 0; i < appDataSize; i++)
  {
    appData[i] = latlong.bytes[i];
  }
  // date-time and alt
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
#if defined(WITH_DEBUG_OUT)
  Serial.begin(115200);
  while (!Serial)
    ;

  uint64_t chipId = 0;
  chipId = ESP.getEfuseMac();                                  //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32ChipID=%04X", (uint16_t)(chipId >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipId);                   //print Low 4bytes.
#endif
  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);
  deviceState = DEVICE_STATE_INIT;
  if (mcuStarted == 0)
  {
    wakeup_display();
    //LoRaWAN.displayMcuInit();

    Display.setFont(ArialMT_Plain_10);
    Display.drawString(0, 1, "WebLoRa");
    Display.display();
  }

  // Battery-Voltage-Input Reading // WIFI not usable then!!!
  analogSetClockDiv(1);                  // Set the divider for the ADC clock, default is 1, range is 1 - 255
  analogSetAttenuation(ADC_11db);        // Sets the input attenuation for ALL ADC inputs, default is ADC_11db, range is ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  analogSetPinAttenuation(36, ADC_11db); // Sets the input attenuation, default is ADC_11db, range is ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  analogSetPinAttenuation(37, ADC_11db);
  adcAttachPin(36);
  adcAttachPin(37);

  delay(2500);
}

// The loop function is called in an endless loop
void loop()
{
  get_coords();   // read coords
  if (validate()) // coordinats are not empty
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
      delay(15000); // show data
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
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
#if defined(WITH_DEBUG_OUT)
    display_data_serial();
#endif
    display_data_8x8();
    delay(2500);
  }
  // }
}