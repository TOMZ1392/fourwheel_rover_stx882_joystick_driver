
#include "joystickDrvDefs.h"
#include "pub_jystk_defs.h"

/* joystick driver interface*/
extern joyStickData_t  joyStickDataList[NUM_JOYSTICKS];

extern void initJoysticks();
extern void Task_readJoystickState();

/* Radio transmitter Interface */
extern void initStx882_tx();
extern void stxIfTransmitData(uint8_t*,uint8_t);




void setup() {
  Serial.begin(9600);
  initStx882_tx();
  initJoysticks();
  delay(1000);
  Serial.println("exiting setup");
}
uint32_t createPacketedData(uint8_t idx,uint16_t x_val,uint16_t y_val,bool sw_state)
{
    volatile uint32_t packet=0;
    packet=x_val&0x3FF;
    packet=packet|(uint32_t)(y_val)<<13;
    packet=(uint32_t)(idx*10)<<26 | packet;
    packet=(uint32_t)(sw_state*10)<<28 | packet;
    return packet;

}
void loop() 
{
  Task_readJoystickState();
  joystickIdentity_t joydata;
  uint8_t idx=0;
  joydata.joystickIdx=idx;
  joydata.x_val=joyStickDataList[idx].x_val;
  joydata.y_val=joyStickDataList[idx].y_val;
  joydata.sw_state=0;
  Serial.print("X Axis val:");
  Serial.println(joydata.x_val);
  stxIfTransmitData((uint8_t*)&joydata,(uint8_t)sizeof(joystickIdentity_t));
  
  



}
