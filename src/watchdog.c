// vim: sw=3 sts=3 expandtab

#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT  PB3   // Pin 2
#define DIAMEX    PB1   // Pin 6
#define RPI_OUT   PB2   // Pin 7
#define RPI_IN    PB4   // Pin 3

#define LOWER_VOLT   192.3 // 12.74V
#define HIGHER_VOLT  194.4 // 12.75V

#define ON  1
#define OFF 0

void initPorts() {
   // init ADC
   ADMUX =
      (1 << ADLAR) |
      (0 << REFS2) |
      (0 << REFS1) |
      (0 << REFS0) |
      (0 << MUX3)  |
      (0 << MUX2)  |
      (1 << MUX1)  |
      (0 << MUX0);

   ADCSRA =
      (1 << ADEN)  |
      (1 << ADPS2) |
      (1 << ADPS1) |
      (0 << ADPS0);

   // init outputs
   DDRB |= (1 << LED_PORT);
   DDRB |= (1 << DIAMEX);
   DDRB |= (1 << RPI_OUT);
   DDRB &= ~(1 << RPI_IN);
   PORTB |= (1 << RPI_IN);   // Enable Pull-Up
}

float readADC() {
   float voltage_fl = 0;
   for (uint8_t sample_loop=150; sample_loop > 0 ; sample_loop--) {
      ADCSRA |= (1 << ADSC);
      while (ADCSRA & (1 << ADSC) );
      voltage_fl += (ADCH - voltage_fl) / 20;
   }
   return voltage_fl;
}

void ledOn() {
   PORTB |= (1 << LED_PORT);
}
void ledOff() {
   PORTB &= ~(1 << LED_PORT);
}

void powerOn() {
      PORTB |= (1 << DIAMEX);
}
void powerOff() {
      PORTB &= ~(1 << DIAMEX);
}

uint8_t getPower() {
   return (PORTB & (1 << DIAMEX)) != 0;
}

void shutdownEnable() {
   PORTB |= (1 << RPI_OUT);
}
void shutdownDisable() {
   PORTB &= ~(1 << RPI_OUT);
}

uint8_t getPiStatus() {
   return (PORTB & (1 << RPI_IN)) == 0;
}


int main(void) {

   initPorts();

   int8_t state_count = 0;

   while(1) {

      ledOff();
      float voltage_fl = readADC();
      ledOn();

      if (voltage_fl > HIGHER_VOLT) {
         if (state_count > 35) {
            if (getPower() == OFF) {
               ledOn();
               powerOn();
               _delay_ms(2000);
               if (getPiStatus() == OFF) {   // RPi didn't boot, reset power
                  powerOff();
                  _delay_ms(1000);
                  powerOn();
               }
            }
            else {
               shutdownDisable();
            }
         }
         else {
            if (state_count < 0) state_count = 0;
            state_count++;
         }
      }
      else if (voltage_fl <= LOWER_VOLT) {
      	 if (state_count < -35) {
            ledOff();
            if (getPiStatus() == ON) {
               shutdownEnable();
            }
            else {
               powerOff();
            }
      	 }
         else {
            if (state_count > 0) state_count = 0;
            state_count--;
      	 }
      }
   }

   return 0;
}

