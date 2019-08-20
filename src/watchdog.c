// vim: sw=3 sts=3 expandtab

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PORT  PB3   // Pin 2
#define POWER     PB1   // Pin 6
#define RPI_OUT   PB2   // Pin 7
#define RPI_IN    PB0   // Pin 5
/*
 * 1 = reset (PB5
 * 2 = LED   (PB3
 * 3 = adc   (PB4
 * 5 =       (PB0
 * 6 = diamex off (PB1
 * 7 = rpi   (PB2
 */

#define LOWER_VOLT   192.3 // 12.74V
#define HIGHER_VOLT  194.4 // 12.75V
#define EMERGENCY_SHUTOFF 180.0

#define ON  1
#define OFF 0

EMPTY_INTERRUPT(ADC_vect);

void initPorts() {
   // init ADC PB4/Pin 3
   ADMUX =
      (1 << ADLAR) |
      (0 << REFS2) |
      (0 << REFS1) |
      (0 << REFS0) |
      (0 << MUX3)  |
      (0 << MUX2)  |
      (1 << MUX1)  | // PB4 / Pin3
      (0 << MUX0);

   ADCSRA =
      (1 << ADEN)  | // Enable ADC
      (1 << ADIE)  | // Enable ADC interrupt
      (1 << ADPS2) |
      (0 << ADPS1) |
      (0 << ADPS0);  // 62,5kHz @ 1MHz

   // init outputs
   DDRB |= (1 << LED_PORT);
   DDRB |= (1 << POWER);
   DDRB |= (1 << RPI_OUT);
   DDRB &= ~(1 << RPI_IN);
   PORTB &= ~(1 << RPI_IN);   // Disable Pull-Up
}

float readADC() {
   float voltage_fl = 0;
   for (uint8_t sample_loop=150; sample_loop > 0 ; sample_loop--) {
      //ADCSRA |= (1 << ADSC);
      set_sleep_mode(SLEEP_MODE_ADC);
      sleep_mode();
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

void shutdownEnable() {
   PORTB |= (1 << RPI_OUT);
}
void shutdownDisable() {
   PORTB &= ~(1 << RPI_OUT);
}

void powerOn() {
   shutdownDisable();
   PORTB |= (1 << POWER);
}
void powerOff() {
   shutdownEnable();
   PORTB &= ~(1 << POWER);
}

uint8_t getPower() {
   return (PORTB & (1 << POWER)) != 0;
}

uint8_t getPiStatus() {
   if ((PINB & (1 << RPI_IN)) == 0)
      return OFF;
   else
      return ON;
}

void delayBlink(uint16_t delay, uint8_t count) {
   for (uint8_t i = 0; i < count; i++) {
      PINB = (1<<LED_PORT);
      for (uint8_t d = 0; d < delay; d++)
         _delay_ms(10);
   }
}

int main(void) {

   initPorts();

   int8_t state_count = 0;
   float voltage_fl;

   while(1) {
      ledOn();
      voltage_fl = readADC();
      ledOff();

      if (voltage_fl <= EMERGENCY_SHUTOFF) {
         powerOff();
         delayBlink(4,25);
      }
      else if (voltage_fl > HIGHER_VOLT) {
         if (state_count > 2) {
            if (getPower() == OFF) {
               powerOn();
               _delay_ms(20000);
               ledOff();
            }
            else if (getPower() == ON && getPiStatus() == OFF) { // RPi didn't boot, reset power
               ledOff();
               powerOff();
               _delay_ms(1000);
               ledOn();
               powerOn();
               _delay_ms(20000);
            }
            else {
               shutdownDisable();
               delayBlink(50,2);
            }
         }
         else {
            if (state_count < 0) state_count = 0;
            state_count++;
            _delay_ms(100);
            ledOff();
            _delay_ms(900);
         }
      }
      else if (voltage_fl <= LOWER_VOLT) {
      	 //if (state_count < -2) {
            if (getPiStatus() == ON) {
               shutdownEnable();
               delayBlink(10,10);
            }
            else {
               powerOff();
               delayBlink(20,5);
            }
      	 //}
         //else {
            if (state_count > 0) state_count = 0;
         //   state_count--;
      	 //}
      }
   }

   return 0;
}

