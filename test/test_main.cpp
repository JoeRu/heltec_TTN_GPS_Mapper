#include <Arduino.h>
#include <unity.h>
#include "heltec_ttn_mapper.h"

int tst_chars = 0;

void test_Serial2(void)
{
    Serial2.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
    delay(3000);
    TEST_ASSERT_TRUE(Serial2.available());
}

void test_Serial_Communication(void)
{
    while (Serial2.available())
        gps.encode(Serial2.read());
     int act_chars = gps.charsProcessed();
     TEST_ASSERT_GREATER_OR_EQUAL_INT(tst_chars + 1, act_chars);
     tst_chars = act_chars;
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
}

void loop()
{
    RUN_TEST(test_Serial2);
    RUN_TEST(test_Serial_Communication); // Test for GPS Serial
    delay(3000);
    RUN_TEST(test_Serial_Communication);
    delay(3000);
    RUN_TEST(test_Serial_Communication);
    UNITY_END(); // stop unit testing
}