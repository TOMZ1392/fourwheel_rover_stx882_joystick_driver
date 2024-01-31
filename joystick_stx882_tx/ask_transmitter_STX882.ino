// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//#define DEBUG_EN 

RH_ASK driver;//TX 12
//RH_ASK driver(2000,9,10);//TX , RX
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

#ifdef DEBUG_EN
void setup()
{



    Serial.begin(9600);	  // Debugging only
    if (!driver.init())

         Serial.println("init failed");


}

void loop()
{
    static uint8_t ctr;
    

    driver.send((uint8_t*)&ctr,1);
    driver.waitPacketSent();
    delay(500);
    ctr++;
}
#endif

void transmitterDiag();

void initStx882_tx()
{
Serial.begin(9600);	  // Debugging only
    if (!driver.init())

         Serial.println("TX init failed");

    volatile unsigned long start=millis();
    while(millis() - start<=5000)
    {
      transmitterDiag();
      
    }


}
void stxIfTransmitData(uint8_t* data,uint8_t payloadBytes)
{
  driver.send(data,payloadBytes);

}


void transmitterDiag()
{
    const char *msg = "hello";

    driver.send((uint8_t *)msg, 6);
    driver.waitPacketSent();
    delay(200);
}
