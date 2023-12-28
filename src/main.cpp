/**
 * File: main.cpp
 * Author: github.com/annadostoevskaya
 * Date: 12/27/2023 03:12:24
 * Last Modified Date: 12/27/2023 03:12:29
 */

#include <avr/io.h>
#include <util/delay.h>

int main()
{

    DDRB |= _BV(DDB0);

    for (;;)
    {
        PORTB ^= _BV(PORTB0);
        _delay_ms(1000.0f);
    }

    return 0;
}
