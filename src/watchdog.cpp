// vim: sw=3 sts=3 expandtab

#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <Adafruit_DotStar.h>

#if defined(ARDUINO_ARCH_SAMD)
 #define PIN_ADC   PIN_A0  // SAMD pin 3
 #define PIN_5VENA PIN_A4  // SAMD pin 7
 #define PIN_PIRUN PIN_A3  // SAMD pin 8
#else
 #error Unsupported hardware
#endif

#define I2C_ADDR  0x8
#define NUMPIXELS 1
#define ON  1
#define OFF 0

Adafruit_DotStar Pixel = Adafruit_DotStar(
  NUMPIXELS, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

uint8_t ShutdownVolt = 208;   // 12.74V
uint8_t StartupVolt  = 210;   // 12.75V
uint8_t EmergencyShutoffVolt = 196; // 11.9V

uint8_t ADCLastVal = 0;
time_t PiWatchdog = 0;
const time_t WatchdogTimeout = 300; // 5 minutes
uint8_t ReadRegister = 0;
uint8_t ShutdownFlag = 0;

void TimerCallback();

void i2c_onRequest() {
   Serial.print("Received I2C Request (");
   Serial.print(ReadRegister);
   Serial.println(")");

   switch (ReadRegister) {
      case 0x1:
         Wire.write(ADCLastVal);
         break;
      case 0x2:
         Wire.write(ShutdownFlag);
         break;
      case 0x10:
         Wire.write(StartupVolt);
         Wire.write(ShutdownVolt);
         Wire.write(EmergencyShutoffVolt);
         break;
      case 0x11:
         Wire.write(StartupVolt);
         break;
      case 0x12:
         Wire.write(ShutdownVolt);
         break;
      case 0x13:
         Wire.write(EmergencyShutoffVolt);
         break;
   }

   if (PiWatchdog) PiWatchdog = now();
}

void i2c_onReceive(int byteCount) {
   uint8_t byte = Wire.read();
   Serial.print("Received I2C count(");
   Serial.print(byteCount);
   Serial.print(") data(");
   Serial.print(byte);
   Serial.println(")");

   switch (byte) {
      case 0x1:
      case 0x2:
         ReadRegister = byte;
         break;
      case 0x03:
         PiWatchdog = 1;
         break;
      case 0x04:
         PiWatchdog = 0;
         break;
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
         ReadRegister = byte;
         break;
      case 0x20:
         StartupVolt = Wire.read();
         ShutdownVolt = Wire.read();
         EmergencyShutoffVolt = Wire.read();
         break;
      case 0x21:
         StartupVolt = Wire.read();
         Serial.print("Received StartupVolt(");
         Serial.print(StartupVolt);
         Serial.println(")");
         break;
      case 0x22:
         ShutdownVolt = Wire.read();
         Serial.print("Received ShutdownVolt(");
         Serial.print(ShutdownVolt);
         Serial.println(")");
         break;
      case 0x23:
         EmergencyShutoffVolt = Wire.read();
         Serial.print("Received EmergencyShutoffVolt(");
         Serial.print(EmergencyShutoffVolt);
         Serial.println(")");
         break;
   }

   while (Wire.available () > 0)
      Wire.read ();
}

void setup() {
   analogReference(AR_DEFAULT);
   analogReadResolution(8);

   pinMode(PIN_5VENA, OUTPUT);
   pinMode(PIN_LED, OUTPUT);
   pinMode(PIN_PIRUN, INPUT_PULLDOWN);
   pinMode(PIN_ADC, INPUT);

   Serial.begin(115200);

   Wire.begin(I2C_ADDR);
   Wire.onRequest(i2c_onRequest);
   Wire.onReceive(i2c_onReceive);

   Pixel.begin();
   Pixel.show();
   Serial.println("Initialized");
}

void ledToggle() {
   digitalWrite(PIN_LED, digitalRead(PIN_LED) != HIGH);
}
void ledOn() {
   digitalWrite(PIN_LED, HIGH);
}
void ledOff() {
   digitalWrite(PIN_LED, LOW);
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

uint8_t filteredAnalogRead(unsigned char channel) {
   uint16_t samples = 0;
   for (uint8_t i = 0; i < 16; i++) {
      samples += analogRead(channel);
   }
   return samples / 16;
}

int8_t StateCount = 0;
int8_t EmergencyCount = 0;

void loop() {
   if (getPiStatus())
      ledOn();
   else
      ledOff();

   ADCLastVal = filteredAnalogRead(PIN_ADC);
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
      if (EmergencyCount > 50) {
         Serial.println("EMERGENCY SHUTDOWN "+String(ADCLastVal));
         setColor(0x7f0000);
         setPowerOff();
         delay(100);
         setColor(0);
         delay(800);
      }
   }
   else if (ADCLastVal > StartupVolt) {
      EmergencyCount = 0;
      if (StateCount > 5) {
         if (getPower() == OFF) {
            Serial.println("Power on Pi "+String(ADCLastVal));
            setColor(0x007f00);
            setPowerOn();
            delay(60000);  // Give Pi time to boot
         }
         else if (getPower() == ON && getPiStatus() == OFF) { // RPi didn't boot, reset setPower
            Serial.println("Reset Pi "+String(ADCLastVal));
            setColor(0x7f7f00);
            setPowerOff();
            delay(1000);
            setColor(0x007f00);
            setPowerOn();
            delay(60000);  // Give Pi time to boot 
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
         else if (getPower() == ON) {
            Serial.println("Power off Pi "+String(ADCLastVal));
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

