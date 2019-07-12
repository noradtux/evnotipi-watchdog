/ vim: sw=3 sts=3 expandtab
#include <avr/io.h>
#include <util/delay.h>
#define LED_PORT PB3 // Pin 2
#define DIAMEX PB1   // Pin 6
#define RPI PB2      // Pin 7

void initADC() {
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
}

int main(void) {
   initADC();
   DDRB |= (1 << LED_PORT);
   DDRB |= (1 << DIAMEX);
   DDRB |= (1 << RPI);

   int count1 = 0;
   int count2 = 0;
   int rpistat = 0;

   while(1) {
      // Blink the LED
      PORTB &= ~(1 << LED_PORT);
      _delay_ms(200);
      PORTB |= (1 << LED_PORT);

      // Read voltage
      float voltage_fl;
      int sample_loop;
      for (sample_loop=150; sample_loop > 0 ; sample_loop --) {
       	 ADCSRA |= (1 << ADSC);
       	 while (ADCSRA & (1 << ADSC) );
       	 voltage_fl = voltage_fl + ((ADCH - voltage_fl) / 20);
      }

      if (voltage_fl > 192.3) {
      	 if (voltage_fl > 194.4) {
            if (count1 > 2) {
               if (rpistat < 1) {
            	  PORTB |= (1 << DIAMEX);
            	  rpistat = 1;
               }
               count1 = 0;

            }
            else {
               count1 ++;
               count2 = 0;
            }
      	 }
         else {
      	    count1 = 0;
      	    count2 = 0;
      	 }
      }
      else {
      	 if (count2 > 35) {
            if (rpistat > 0) {
               PORTB |= (1 << RPI);
               _delay_ms(4000);
               PORTB &= ~(1 << RPI);
               _delay_ms(35000);
               PORTB &= ~(1 << DIAMEX);
               rpistat = 0;
            }
            count1 = 0;
            count2 = 0;
      	 }
         else {
            count2 ++;
            count1 = 0;
            PORTB &= ~(1 << LED_PORT);
            _delay_ms(5000);
      	 }
      }
   }

   return 0;
}

