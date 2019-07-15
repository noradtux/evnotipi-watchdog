// vim: sw=3 sts=3 expandtab

#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT  PB3   // Pin 2
#define POWER     PB1   // Pin 6
#define RPI_OUT   PB2   // Pin 7
#define RPI_IN    PB4   // Pin 3

#define LOWER_VOLT   192.3 // 12.74V
#define HIGHER_VOLT  194.4 // 12.75V
#define EMERGENCY_SHUTOFF 180.0

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
   DDRB |= (1 << POWER);
   DDRB |= (1 << RPI_OUT);
   DDRB &= ~(1 << RPI_IN);
   PORTB &= ~(1 << RPI_IN);   // Disable Pull-Up
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
   return (PORTB & (1 << RPI_IN)) == 0;
}

int main(void) {

   initPorts();

   int8_t state_count = 0;
   float voltage_fl;

   while(1) {
      ledOn();
      voltage_fl = readADC();

      if (voltage_fl <= EMERGENCY_SHUTOFF) {
         powerOff();
         _delay_ms(900);
         ledOff();
         _delay_ms(100);
      }
      else if (voltage_fl > HIGHER_VOLT) {
         if (state_count > 2) {
            if (getPower() == OFF) {
               powerOn();
               _delay_ms(2000);
               ledOff();
            }
            else if (getPower() == ON && getPiStatus() == OFF) { // RPi didn't boot, reset power
               powerOff();
               _delay_ms(1000);
               powerOn();
               _delay_ms(500);
               ledOff();
               _delay_ms(500);
            }
            else {
               shutdownDisable();
               _delay_ms(500);
               ledOff();
               _delay_ms(500);
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
               _delay_ms(100);
               ledOff();
               _delay_ms(100);
               ledOn();
               _delay_ms(100);
               ledOff();
               _delay_ms(700);
            }
            else {
               powerOff();
               _delay_ms(100);
               ledOff();
               _delay_ms(300);
               ledOn();
               _delay_ms(100);
               ledOff();
               _delay_ms(500);
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

