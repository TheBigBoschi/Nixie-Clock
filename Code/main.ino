#include <Wire.h>
#include <EEPROM.h>
#include <FaBoRTC_PCF2129.h>

void writeTime(byte A, byte B, byte DOT);  //A=hours, B=minutes, DOT=dot status (0=off, 1=on)
byte slotMachine(byte H, byte M, byte rollTime);  //H=hours, M=minutes, rollTime=total rolling time in seconds
byte getButton1();  //Returns a boolean value indicating if button 1 was pressed since last call
byte getButton2();  //Returns a boolean value indicating if button 2 was pressed since last call
byte getButton3();  //Returns a boolean value indicating if button 3 was pressed since last call


FaBoRTC_PCF2129 faboRTC;

//Pin definitions
#define LATCH PIN_PA6 //Latch pin for the shift register
#define PWM PIN_PA5 //PWM pin for the brightness control
#define CLOCK PIN_PA4 //Clock pin for the shift register
#define DATA PIN_PA3 //Data pin for the shift register
#define JS1 PIN_PB5 //spare pin 1
#define BUTTON1 PIN_PB3 //Button 1 physical pin
#define BUTTON2 PIN_PB2 //Button 2 physical pin
#define BUTTON3 PIN_PB1 //Button 3 physical pin

#define WAITDELAY_LONG 1500 //Milliseconds after wich the button starts repeating presses
#define WAITDELAY_SHORT 150 //Milliseconds between repeated presses


#define SLOT_DELAY 20 //Delay between each slot machine digit change in milliseconds
#define SLOT_DELAY_ROLL 20 //Delay between each slot machine digit change during the final roll-in phase in milliseconds
#define SLOT_DELAY_BLINK 100  //Delay for the blinking effect during slot machine rolling in milliseconds
#define SLOT_TRESHOLD 100 //Minimum difference between current time and final time to perform the roll-in effect
#define SLOT_NUM 11 //Number of digits the slot machine advances at each step
#define SLOT_MUL 1  //Multiplier for the slot machine time between number increments in the final phase, the higher the number the slower it rolls

byte ledStat = 0;
byte pwm = 0;
bool dayLightSaving = true;
bool dayLightSavingStat = 0;
byte timeShift = 0;
bool settingFlag;
bool blinking;

byte randHour;
byte randMin;

void buttonsCheck();


void setup()
{
  Wire.pins(PIN_PA1, PIN_PA2);  //Initialise the I2C pins on PA1 and PA2 

  //Initializes the GPIO pins

  pinMode(LATCH, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(JS1, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  analogWrite(PWM, 0);

  //Initializes the RTC

  faboRTC.searchDevice();
  faboRTC.configure();

  //Initializes for the first time the BurnIn function

  randomSeed(pwm);                                      
  randHour = (random(8,12) + faboRTC.getHours())%24;
  randMin = 30;

  //Restore from the eeprom the saved settings
  restoreSettings();   
  //Checks the buttons status
  buttonsCheck();
  //Is used as a workaround. the first time the function gets called, it return as the button is pressed. From the second time it works normally.
  getButton3();


  if(faboRTC.getSeconds() > 60 )    //If seconds > 60 the RTC has lost backup power and the time has to be set again. Read about the register 03h of the PCF2129 datasheet for more info.
    initializeClock();
  
  //This two elses are used to detect if a daylight saving time change has occurred while the clock was powered off
  else if(faboRTC.getMonths() == 3 && faboRTC.getDays()==isLastSundayOfTheMonthPassed() && faboRTC.getHours() == 2 && faboRTC.getMinutes() == 0 && dayLightSaving && dayLightSavingStat == 0)  //se durante il cambio dell'ora legale l'orologio è spento questa routine al primo avvio corregge l'orario
    {
      faboRTC.setHours(faboRTC.getHours() + 1);
      dayLightSavingStat = 1;
      EEPROM.update(4,dayLightSavingStat);
    }
  else if(faboRTC.getMonths() == 10 && faboRTC.getDays()==isLastSundayOfTheMonthPassed() && faboRTC.getHours() == 3 && faboRTC.getMinutes() == 0 && dayLightSaving && dayLightSavingStat == 1)
    {
      faboRTC.setHours(faboRTC.getHours() - 1);
      dayLightSavingStat = 0;
      EEPROM.update(4,dayLightSavingStat);
    }
}

void loop() {

     
  buttonsCheck(); //Refreshes the buttons status
 
  //If it's time to execute the BurnIn routine, first sets the next routine time, then it executes it.
  if(randHour == faboRTC.getHours() && randMin == faboRTC.getMinutes())
    {
      randHour = (randHour + random(8,12))%24;
      randomSeed(faboRTC.getMonths()+100*faboRTC.getHours()+faboRTC.getDays()*10000); 
      slotMachine((int)random(5,10));
    }
      
  if(getButton3())
    settings();
  
  //Refreshes the written time. 
  writeTime(faboRTC.getHours(),faboRTC.getMinutes(),blinking && (millis()/1000)%2); 
}
