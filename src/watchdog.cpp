// vim: sw=3 sts=3 expandtab

#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <EEPROM.h>
#include <Adafruit_DotStar.h>
//#include <avr/setPower.h>

#if 1
#define PIN_ADC   PIN_A0
#define PIN_5VENA PIN_A4
#define PIN_PIRUN PIN_A3
#else
#define PIN_ADC   PB1
#define PIN_5VENA PB4
#define PIN_PIRUN PB3
#endif

#define I2C_ADDR  2
#define NUMPIXELS 1

#define EEPROM_START 1
#define EEPROM_SHUT 2
#define EEPROM_EMERG 3
unsigned char ShutdownVolt = 208   // 12.74V
unsigned char StartupVolt  = 210   // 12.75V
unsigned char EmergencyShutoffVolt = 196 // 11.9V

#define ON  1
#define OFF 0

Adafruit_DotStar Pixel = Adafruit_DotStar(
  NUMPIXELS, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

unsigned char ADCLastVal = 0;
time_t PiWatchdog = 0;
const time_t WatchdogTimeout = 300; // 5 minutes
unsigned char ReadRegister = 0;
unsigned char ShutdownFlag = 0;

void i2c_onRequest() {
   Serial.print("Received I2C Request (");
   Serial.print(ReadRegister);
   Serial.println(")");
   Wire.write(ADCLastVal);
   Wire.write(ShutdownFlag);
   if (PiWatchdog) PiWatchdog = now();
}

void i2c_onReceive(int byteCount) {
   Serial.print("Received I2C data (");
   Serial.print(byteCount);
   Serial.println(")");
   char byte = Wire.read();
   switch (byte) {
      case 0x1:
         PiWatchdog = 1;
         ;;
      case 0x2:
         PiWatchdog = 0;
         ;;
      case 0x3:
         StartupVolt = Wire.read();
         EEPROM.write(EEPROM_START, StartupVolt);
         ShutdownVolt = Wire.read();
         EEPROM.write(EEPROM_SHUT, ShutdownVolt);
         EmergencyShutoffVolt = Wire.read()
         EEPROM.write(EEPROM_EMERG, EmergencyShutoffVolt);
         Serial.print(" New voltage levels Start(");
         Serial.print(StartupVolt);
         Serial.print(") Shut(");
         Serial.print(ShutdownVolt);
         Serial.print("Emerg(");
         Serial.print(EmergencyShutoffVolt);
         Serial.println(")");
         ;;
   }
}

void setup() {
   analogReference(AR_DEFAULT);
   analogReadResolution(8);

   pinMode(PIN_5VENA, OUTPUT);
   pinMode(PIN_LED, OUTPUT);
   pinMode(PIN_PIRUN, INPUT_PULLUP);
   pinMode(PIN_ADC, INPUT);

   Serial.begin(115200);

   Wire.begin(I2C_ADDR);
   Wire.onRequest(i2c_onRequest);
   Wire.onReceive(i2c_onReceive);

   unsigned char v;
   v = EEPROM.read(EEPROM_SHUT);
   if (v) ShutdownVolt = v;

   v = EEPROM.read(EEPROM_START);
   if (v) StartupVolt = v;
   
   v = EEPROM.read(EEPROM_EMERG);
   if (v) EmergencyShutdownVolt = v;
   
   Pixel.begin();
   Pixel.show();
}

void ledToggle() {
   digitalWrite(PIN_LED, digitalRead(PIN_LED) != HIGH);
}

void shutdownEnable() {
   ShutdownFlag = 1;
}
void shutdownDisable() {
   ShutdownFlag = 0;
}

void setPowerOn() {
   shutdownDisable();
   digitalWrite(PIN_5VENA, HIGH);
}
void setPowerOff() {
   shutdownEnable();
   digitalWrite(PIN_5VENA, LOW);
}

uint8_t getPower() {
   return digitalRead(PIN_5VENA) == HIGH;
}

uint8_t getPiStatus() {
   return digitalRead(PIN_PIRUN) == HIGH;
}

void setColor(unsigned long int color) {
   Pixel.setPixelColor(0, color);
   Pixel.show();
}
void toggleRGB(unsigned long int color) {
   static int toggle = 0;
   toggle = !toggle;
   if (toggle) {
      setColor(color);
   }
   else {
      setColor(0);
   }
}

int8_t StateCount = 0;
int8_t EmergencyCount = 0;

void loop() {

   ledToggle();
   ADCLastVal = analogRead(PIN_ADC);
   //Serial.println(ADCLastVal);

   if (PiWatchdog && now() - PiWatchdog > WatchdogTimeout) {
      setColor(0x7f7f00);
      setPowerOff();
      delay(300);
      setColor(0x00ff00);
      setPowerOn();
   }

   if (ADCLastVal <= EmergencyShutoffVolt) {
      EmergencyCount++;
      if (EmergencyCount > 5) {
         Serial.println("EMERGENCY SHUTDOWN "+String(ADCLastVal));
         setColor(0x7f0000);
         setPowerOff();
      }
   }
   else if (ADCLastVal > StartupVolt) {
      EmergencyCount = 0;
      if (StateCount > 5) {
         if (getPower() == OFF) {
            Serial.println("Power on Pi "+String(ADCLastVal));
            setColor(0x007f00);
            setPowerOn();
            delay(20000);
         }
         else if (getPower() == ON && getPiStatus() == OFF) { // RPi didn't boot, reset setPower
            Serial.println("Reset Pi "+String(ADCLastVal));
            setColor(0x7f7f00);
            setPowerOff();
            delay(1000);
            setColor(0x007f00);
            setPowerOn();
            delay(20000);
         }
         else {
            //Serial.println("Pi running "+String(ADCLastVal));
            shutdownDisable();
            toggleRGB(0x007f00);
         }
      }
      else {
         if (StateCount < 0) StateCount = 0;
         StateCount++;
         //setColor(0x00a0a0);
      }
   }
   else if (ADCLastVal <= ShutdownVolt) {
      EmergencyCount = 0;
      if (StateCount < -5) {
         if (getPiStatus() == ON) {
            Serial.println("Request shutdown Pi "+String(ADCLastVal));
            shutdownEnable();
            toggleRGB(0x7f7f00);
         }
         else {
            Serial.println("Pi off "+String(ADCLastVal));
            setPowerOff();
            setColor(0);
         }
      }
      else {
         if (StateCount > 0) StateCount = 0;
         StateCount--;
      }
   }
   delay(100);
}

