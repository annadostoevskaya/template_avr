/**
 * File: main.cpp
 * Author: github.com/annadostoevskaya
 * Date: 12/27/2023 03:12:24
 * Last Modified Date: 12/27/2023 03:12:29
 */

#include <avr/io.h>
#include <avr/interrupt.h>

ISR (TIMER0_OVF_vect)
{
    PORTB ^= _BV(PORTB0);
    TCNT0 = 0;
}

int main()
{
    /*
    Title: interrupt & TCNT0
    F_CPU: 9.6MHz
    Prescaler: 1024
    TCNT0: 8bit Timer/Counter, Max - 255
    9.6e6 Hz / 1024 = 9375 Hz

    NB(annad): You are must set CKDIV8 in logic 0!
        It's meaning set fuse bits as `-U lfuse:w:0xe4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m`
    */

    DDRB |= _BV(DDB0);

    TCNT0 = 0;
    TCCR0B |= (1 << CS00 | 1 << CS02);
    TCCR0A = 0x00;

    TIMSK |= 1 << TOIE0;

    sei();

    for (;;) {}

    return 0;
}
