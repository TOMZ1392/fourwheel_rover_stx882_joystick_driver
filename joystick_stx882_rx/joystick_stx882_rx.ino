
#include "pub_jystk_defs.h"
joystickIdentity_t jydataBuf;

/* Radio reciever Interface */
extern void initStx882_rx();
extern bool stx882IfGetRecievedData(uint8_t* data,size_t readSize);

/* battery monitor interface*/
extern void  batterypackMonInit();
extern void Task_BatteryPackMonitor();
extern bool isBatterypackGood();
extern uint8_t getBattLvlPercent();

/*power train interface*/
extern void initPwrTrain();
extern void adapter_joystickToPwrTrn(uint16_t x,uint16_t y);



void setup() {
  Serial.begin(9600);
  batterypackMonInit();	
  initStx882_rx();
  initPwrTrain();
  Serial.println("exiting setup");
}

void loop() {
  Task_BatteryPackMonitor();
  if(isBatterypackGood())
  {
    //Serial.println("Batt Health good!");
      if(stx882IfGetRecievedData((uint8_t*)&jydataBuf,(uint8_t)(sizeof(joystickIdentity_t)))==1)
    {
      //Serial.println(jydataBuf.x_val);
      adapter_joystickToPwrTrn(jydataBuf.x_val,jydataBuf.y_val);

    }


  }
  else{

    Serial.println("Waiting for BatterPack status!!!");
    
  }
   //Serial.println("Next cyc");
}
