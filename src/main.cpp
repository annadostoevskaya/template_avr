#include <avr/io.h>
#include <avr/interrupt.h>
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
