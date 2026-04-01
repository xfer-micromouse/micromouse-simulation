// Define the clock speed so the delay function knows how to calculate time
// 16 MHz is the standard speed for an Arduino Nano/Uno
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // SETUP: Equivalent to pinMode(7, OUTPUT);
    // Set the 7th bit of the Data Direction Register for Port D to 1 (Output)
    DDRD |= (1 << PD7);

    // LOOP:
    while (1) {
        // Equivalent to digitalWrite(7, HIGH);
        // Set the 7th bit of the PORTD register to 1
        PORTD |= (1 << PD7);

        // Wait 500 milliseconds
        _delay_ms(1);

        // Equivalent to digitalWrite(7, LOW);
        // Set the 7th bit of the PORTD register to 0 by using bitwise AND with a NOT mask
        PORTD &= ~(1 << PD7);

        // Wait 500 milliseconds
        _delay_ms(1);
    }

    return 0;
}