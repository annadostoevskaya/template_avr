/**
 * File: main.cpp
 * Author: github.com/annadostoevskaya
 * Date: 12/27/2023 03:12:24
 * Last Modified Date: 12/27/2023 03:12:29
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "RingBuffer.h"

const int16_t g_UBRR = (F_CPU / (16 * BAUD)) - 1;

int uart_putchar(char c, FILE *stream)
{
    while (!(UCSRA & _BV(UDRE)))
        ;;

    TXB = c;

    return 0;
}

FILE g_UART_Stream = {
    NULL,
    0,
    _FDEV_SETUP_RW,
    0,
    0,
    uart_putchar,
    NULL,
    0
}; 

RingBuffer<16> g_UART_Buffer;

ISR(USART_RX_vect) { g_UART_Buffer.write(RXB); }

int main()
{
    stdout = &g_UART_Stream;

    set_sleep_mode(SLEEP_MODE_IDLE);

    UBRRL = g_UBRR;
    UBRRH = g_UBRR >> 8;
    UCSRB |= _BV(TXEN) | _BV(RXEN) | _BV(RXCIE);
    UCSRC = _BV(UCSZ1) | _BV(UCSZ0);

    sei();
    
    for (;;) 
    {
        sleep_mode();

        while (g_UART_Buffer.available())
        {
            if (g_UART_Buffer.read() == 'd')
            {
                for (size_t i = 0; i < sizeof(g_UART_Buffer.m_Buffer) / sizeof(*g_UART_Buffer.m_Buffer); i += 1)
                {
                    if (i % 8 == 0) putchar('\n');
                    printf("%3X ", g_UART_Buffer.m_Buffer[i]);
                }
                putchar('\n');
            }

            printf("%d:%d\n", g_UART_Buffer.m_Head, g_UART_Buffer.m_Tail);
        }
    }
    
    return 0;
}
