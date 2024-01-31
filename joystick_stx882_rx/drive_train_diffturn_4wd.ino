/* PowerTrain out digital out pin defines*/

#define FWD_LFT   6  
#define FWD_RGT   7  
#define REV_LFT   8  
#define REV_RGT   9 

/* PowerTrain out pwm out pin defines*/
#define PWMRGT    3
#define PWMLFT    5


/* PWM CALIBRATION*/
#define PWM_STEP 50
#define PWM_LOLIM 50
#define PWM_UPLIM 970
#define PWM_MIN_LIM 50  // verify power consumption


#define X_FWD_CUTTOFF_ADA  515
#define X_REV_CUTTOFF_ADA  530
#define Y_CCW_CUTTOFF_ADA  500
#define Y_CW_CUTTOFF_ADA  515



//#define TEST_BLOCK_EN
//#define SERIAL_DEBUG_PT

#ifdef SERIAL_DEBUG_PT
  #define SERIAL_BEGIN(x)     {Serial.begin(x);}
  #define SERIAL_PRNT(x)      {Serial.print(x);}
  #define SERIAL_PRNTLN(x)    {Serial.println(x);}
#else
  #define SERIAL_BEGIN(x)   {}
  #define SERIAL_PRNT(x)    {}
  #define SERIAL_PRNTLN(x)  {}
#endif

void setMotorPwm(uint16_t lft, uint16_t rgt) 
{
    if (lft <= PWM_LOLIM)
        lft = PWM_MIN_LIM;
    if (lft >= PWM_UPLIM)
        lft = PWM_UPLIM;
    if (rgt <= PWM_LOLIM)
        rgt = PWM_MIN_LIM;
    if (rgt >= PWM_UPLIM)
        rgt = PWM_UPLIM;
    uint8_t pwmLft = lft;
    uint8_t pwmRgt = rgt;
   
      SERIAL_PRNT("Lftpwm: ");
      SERIAL_PRNT(pwmLft);
      SERIAL_PRNT(" Rgtpwm: ");
      SERIAL_PRNTLN(pwmRgt);
    
    analogWrite(PWMLFT, lft);
    analogWrite(PWMRGT, rgt);
}

// stop drive immediately

void pullAllLow() 
{
    digitalWrite(REV_LFT, LOW);
    digitalWrite(REV_RGT, LOW);
    digitalWrite(FWD_LFT, LOW);
    digitalWrite(FWD_RGT, LOW);
    //delay(200); -- need a ramp instead
}

void fwd()
{

    digitalWrite(FWD_LFT, LOW);//0
    digitalWrite(FWD_RGT, HIGH);//1
    digitalWrite(REV_LFT, LOW);//0
    digitalWrite(REV_RGT, HIGH);//1
    
}

void rev()
{
    digitalWrite(FWD_LFT, HIGH);// 1
    digitalWrite(FWD_RGT, LOW);//0
    digitalWrite(REV_LFT, HIGH);//1
    digitalWrite(REV_RGT, LOW);//0

}

void cwRot()
{
    pullAllLow();
    digitalWrite(FWD_LFT, HIGH); //6  1
    digitalWrite(FWD_RGT, LOW); //7   0
    digitalWrite(REV_LFT, LOW); //8   0
    digitalWrite(REV_RGT, HIGH); //9  1
    
}
void ccwRot()
{
    pullAllLow();
    digitalWrite(FWD_LFT, LOW); // 0
    digitalWrite(FWD_RGT, HIGH);//1
    digitalWrite(REV_LFT, HIGH);//1
    digitalWrite(REV_RGT, LOW);//0
    
}

void definePinModes()
{
    
   
    pinMode(13, OUTPUT); //d7

    pinMode(FWD_LFT, OUTPUT);  //d2
    pinMode(FWD_RGT, OUTPUT);  //d3
    pinMode(REV_LFT, OUTPUT);  //d4
    pinMode(REV_RGT, OUTPUT); //d5
    
    pinMode(PWMLFT, OUTPUT); //d6
    pinMode(PWMRGT, OUTPUT);  //d1
}

void initPinsLow()
{
    digitalWrite(PWMRGT, LOW);  //d1 pwm
    digitalWrite(PWMLFT, LOW); //d6 pwm
    digitalWrite(FWD_LFT, LOW);  //d2
    digitalWrite(FWD_RGT, LOW);  //d3
    digitalWrite(REV_LFT, LOW);  //d4
    digitalWrite(REV_RGT, LOW); //d5
    digitalWrite(13, LOW); //d7
}

// initialization
void initPwrTrain()
{
  
  definePinModes();
  initPinsLow();
}

void adapter_joystickToPwrTrn(uint16_t x,uint16_t y)
{
  bool isStopped=true;
  bool headingRequested=false;
  SERIAL_PRNT("X VAL: ");
  SERIAL_PRNT(x);
  SERIAL_PRNT("  Y VAL: ");
  SERIAL_PRNTLN(y);
  //delay(1000);
  uint8_t pwmLft=0,pwmRgt=0;

  if(x > X_FWD_CUTTOFF_ADA && x < X_REV_CUTTOFF_ADA )
  {
    pwmLft=PWM_LOLIM;
    pwmRgt=PWM_LOLIM;
    setMotorPwm(PWM_LOLIM, PWM_LOLIM);
    isStopped=false;
  }

  if(x < X_FWD_CUTTOFF_ADA)
  {
    
    uint8_t x8= map(x, 0, 523, 30, 255);
    pwmLft=255 - x8;
    pwmRgt=255 - x8;
    setMotorPwm(pwmLft, pwmRgt);
    fwd();
    SERIAL_PRNTLN("FWD");
    SERIAL_PRNT("pwm calc lft ");SERIAL_PRNT(pwmLft);SERIAL_PRNT("  rgt ");SERIAL_PRNTLN(pwmRgt);
    //delay(1000);
    headingRequested=true;

  }
  else if(x > X_REV_CUTTOFF_ADA)
  { 
      uint8_t x8= map(x, 523, 1023, 30, 255);
      pwmLft=x8;
      pwmRgt=x8;
      setMotorPwm( pwmLft, pwmRgt);
      rev();
      SERIAL_PRNTLN("REV");
      SERIAL_PRNT("pwm calc lft ");SERIAL_PRNT(pwmLft);SERIAL_PRNT("  rgt ");SERIAL_PRNTLN(pwmRgt);
      //delay(1000);
      headingRequested=true;

  }
  else
  {
    //pullAllLow();
    isStopped=true;
  }
 if(!headingRequested)
 {
  if(y < Y_CCW_CUTTOFF_ADA)
  {
    uint8_t y8= map(y, 523, 1000, 30, 255);
    SERIAL_PRNTLN(y8);
    pwmLft=255 - y8;
    pwmRgt=255 - y8;
    setMotorPwm(pwmLft, pwmRgt);
    cwRot();
    SERIAL_PRNTLN("RGT ROT");
    SERIAL_PRNT("pwm calc lft ");SERIAL_PRNT(pwmLft);SERIAL_PRNT("  rgt ");SERIAL_PRNTLN(pwmRgt);

  }
  else if(y > Y_CW_CUTTOFF_ADA)
  {
      uint8_t y8= map(y, 0, 523, 30, 255);
      pwmLft=y8;
      pwmRgt=y8;
      setMotorPwm( pwmLft, pwmRgt);
      ccwRot();
      SERIAL_PRNTLN("LFT ROT");
      SERIAL_PRNT("pwm calc lft ");SERIAL_PRNT(pwmLft);SERIAL_PRNT("  rgt ");SERIAL_PRNTLN(pwmRgt);

  }
  else
  {
      if(isStopped)
      {
        pullAllLow();
      }
  }
  
 }

}

#ifdef TEST_BLOCK_EN
volatile uint16_t x_val=523,y_val=507;

void setup()
{
  SERIAL_BEGIN(9600);
  initPwrTrain();
}

void loop()
{
  if(Serial.available())
  {
    char c=Serial.read();
    if(c=='F')
    {
      x_val=25;
      y_val=507;
    }
    if(c=='B')
    {
      x_val=1000;
      y_val=507;
    }
    if(c=='L')
    {
      x_val=523;
      y_val=100;
    }
    if(c=='R')
    {
      x_val=523;
      y_val=800;
    }
    if(c=='S')
    {
      x_val=523;
      y_val=507;
    }
  }
  adapter_joystickToPwrTrn(x_val,y_val);
}
#endif
