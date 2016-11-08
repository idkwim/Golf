// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include "Adafruit_SleepyDog.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SPI.h>
#include <RH_RF95.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

#define LED 13
int analogPin = A9;
float batteryVal = 0;

#define sendBufSize 51

char sendBuf[sendBufSize];

 
/* for feather32u4 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
 
 
// Change to 434.0 or other frequency, must match RX's freq!
// #define RF95_FREQ 915.0
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);



void setup() 
{
  int i;
  for (i = 0 ; i < sendBufSize; i++)
  {
    sendBuf[i] =0;
  }


  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  //while (!Serial);

  Serial.begin(115200);
  delay(100);
 
  Serial.println("Feather LoRa TX Test! & BME280");

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 
void loop()
{
  digitalWrite(LED, HIGH);
//  rf95.sleep();
//  Watchdog.sleep(1000);
  
  
//  Serial.print("Temperature = ");
//  Serial.print(bme.readTemperature());
//  Serial.println(" *C");
//
//  Serial.print("Pressure = ");
//
//  Serial.print(bme.readPressure() / 100.0F);
//  Serial.println(" hPa");
//
//  Serial.print("Approx. Altitude = ");
//  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//  Serial.println(" m");
//
//  Serial.print("Humidity = ");
//  Serial.print(bme.readHumidity());
//  Serial.println(" %");


  batteryVal = analogRead(A9) * 6.6 / 1024.0;
  Serial.print(batteryVal);  
  Serial.println(" V");


  String tempstr = String("");
  tempstr += "T: ";
  tempstr += bme.readTemperature();
  tempstr += ", ";
  tempstr += "P: ";
  tempstr += bme.readPressure() / 100.0F;
  tempstr += ", ";
  tempstr += "A: ";
  tempstr += bme.readAltitude(SEALEVELPRESSURE_HPA);
  tempstr += ", ";
  tempstr += "H: ";
  tempstr += bme.readHumidity();
  tempstr += ", ";
  tempstr += "B: ";
  tempstr += batteryVal;
  

  
  tempstr.toCharArray(sendBuf, sendBufSize);
  rf95.send((uint8_t *)sendBuf, tempstr.length());
  delay(10);
  rf95.waitPacketSent();

  Serial.println("Data Sent!");
  

//  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//  uint8_t len = sizeof(buf);
//  int i;
//  for (i = 0 ; i < RH_RF95_MAX_MESSAGE_LEN ; i++)
//  {
//    buf[i] = 0;
//  }
 
//  Serial.println("Waiting for reply..."); delay(10);
//  if (rf95.waitAvailableTimeout(1000))
//  { 
//    // Should be a reply message for us now   
//    if (rf95.recv(buf, &len))
//   {
//      digitalWrite(LED, HIGH);
//      
//      Serial.print("Got reply: ");
//      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);
//      Serial.println();  
//      
//      digitalWrite(LED, LOW);
//    }
//    else
//    {
//      Serial.println("Receive failed");
//    }
//  }
//  else
//  {
//    Serial.println("No reply, is there a listener around?");
//  }
  digitalWrite(LED, LOW);
  delay(1000);
  
}


