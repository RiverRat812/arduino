#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-03-19 17:54:45

#include "Arduino.h"
#include<CountUpDownTimer.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <MemoryFree.h>
void setup(void) ;
void loop(void) ;
void connectToWifi(void)  ;
bool displayConnectionDetails(void) ;
void getHueLightState(int lightNum) ;
void putHueLightState(int lightNum) ;
int GetSensePinValue(byte SlaveDeviceId) ;
void serialDebugPrint() ;
unsigned long getTime(void) ;
void showLCDTime() ;

#include "HueMasterNode0_11.ino"


#endif
