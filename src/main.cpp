#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void setup_clkpr()
{
    cli();
    CLKPR |= (1 << CLKPCE);
    CLKPR |= ((1 << CLKPS2) && (1 << CLKPS1));
    sei();
}

int main()
{
    // setup_clkpr();

    DDRB |= 1 << DDB0;

    for (;;)
    {
        PORTB ^= (1 << PORTB0);
        _delay_ms(1000.0f);
    }

    return 0;
}
