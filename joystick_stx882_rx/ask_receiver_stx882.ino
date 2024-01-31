// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//#define TESTMODE
RH_ASK driver; //11 RX


//BAUD,RX,TX,0

 //RH_ASK driver(2000, 10, 2, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS
#ifdef TEST_MODE
void setup()
{
  pinMode(13,OUTPUT);

    Serial.begin(9600);	  // Debugging only
    

    if (!driver.init())

         Serial.println("init failed");



}

void loop()
{
    
    uint8_t data=0;
    uint8_t dataSize=sizeof(data);
    digitalWrite(13,LOW);

    if (driver.recv((uint8_t*)&data, &dataSize))
    {
      Serial.print(data);
       Serial.print(" ");
	    if(data%3==0)
	    {
        Serial.println(data);
        digitalWrite(13,HIGH);
        delay(300);
	    }

    }
    
}
#endif
void recieverDiag();
void initStx882_rx()
{
    Serial.begin(9600);	  // Debugging only
    

    if (!driver.init())

         Serial.println("RX init failed");
    unsigned long start=millis();
    while(millis() - start<=5000)
    {
      recieverDiag();
      
    }


}

bool stx882IfGetRecievedData(uint8_t* data,uint8_t readSize)
{
   //Serial.println("Inside recieve");
  if (driver.recv(data, &readSize))
    {
        return 1;
    }
    else
    {
      return 0;
    }

}

void recieverDiag()
{
    
    char buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    digitalWrite(13,LOW);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
	int i;
digitalWrite(13,HIGH);
	// Message with a good checksum received, dump it.
  Serial.println(buf);
	//driver.printBuffer("Got:", buf, buflen);
    }
    
}

