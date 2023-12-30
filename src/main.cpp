/**
 * File: main.cpp
 * Author: github.com/annadostoevskaya
 * Date: 12/27/2023 03:12:24
 * Last Modified Date: 12/27/2023 03:12:29
 */

#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

class UART
{
public:
/*
    void print(const char* str)
    {
        for (; *str != '\0'; str++)
            write(*str);
    }
*/
    void write(char c)
    {
        while (!(UCSRA & _BV(UDRE)))
            ;;

        TXB = c;
    }

    uint8_t read()
    {
        while (!(UCSRA & _BV(RXC)))
            ;;

        return RXB;
    }
};

const int16_t g_UBRR = (F_CPU / (16 * BAUD)) - 1;

UART g_UART;

// ISR(USART_RX_vect) { }

int main()
{
    UBRRL = g_UBRR;
    UBRRH = g_UBRR >> 8;
    UCSRB |= _BV(TXEN) | _BV(RXEN);
    UCSRC = _BV(UCSZ1) | _BV(UCSZ0);

    // UCSRC |= _BV(RXCIE);
    // sei();

    for (;;)
    {
        g_UART.write(g_UART.read());
    }

    return 0;
}
