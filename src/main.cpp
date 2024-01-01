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

FILE g_UART_OStream = { // NOTE(annad): Obviously, stdio.h only for debugging purpose.
    NULL,
    0,
    _FDEV_SETUP_RW,
    0,
    0,
    uart_putchar,
    NULL,
    0
}; 

RingBuffer<16> g_UART_IBuffer;

ISR(USART_RX_vect) { g_UART_IBuffer.write(RXB); }

int main()
{
    stdout = &g_UART_OStream;

    set_sleep_mode(SLEEP_MODE_IDLE);

    UBRRL = g_UBRR;
    UBRRH = g_UBRR >> 8;
    UCSRB |= _BV(TXEN) | _BV(RXEN) | _BV(RXCIE);
    UCSRC = _BV(UCSZ1) | _BV(UCSZ0);

    sei();
    
    for (;;) 
    {
        sleep_mode();


    }
    
    return 0;
}
