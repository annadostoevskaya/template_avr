#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRB |= 1 << DDB0;

    for (;;)
    {
        PORTB |= (1 << PORTB0);
        _delay_ms(1000);
        PORTB &= ~(1 << PORTB0);
        _delay_ms(1000);
    }

    return 0;
}
