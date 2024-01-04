/**
 * File: main.cpp
 * Author: github.com/annadostoevskaya
 * Date: 01/04/2023 08:12:24
 * Last Modified Date 01/04/2023 08:12:29
 */

#include <stdint.h>
#include <stdio.h> // NOTE(annad): [!] WARNING: `avrdude: writing 2002 bytes flash`
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "Serial.hpp"
#include "RingBuffer.hpp"

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
using TRingBuffer = decltype(g_UART_IBuffer);
ISR(USART_RX_vect) { g_UART_IBuffer.write(RXB); }

Serial g_OSerial;

template<typename T>
T serial_get_data(TRingBuffer& IBuffer)
{
    T data;
    size_t byte_count = 0;
    while (byte_count != sizeof(T))
    {
        if (IBuffer.available())
        {
            (reinterpret_cast<uint8_t*>(&data))[byte_count] = IBuffer.read();
            byte_count += 1;
        }
    }

    return data;
}

int main()
{
    stdout = &g_UART_OStream;

    set_sleep_mode(SLEEP_MODE_IDLE);

    UBRRL = g_UBRR;
    UBRRH = g_UBRR >> 8;
    UCSRB |= _BV(TXEN) | _BV(RXEN) | _BV(RXCIE);
    UCSRC = _BV(UCSZ1) | _BV(UCSZ0);

    sei();
    
    _delay_ms(1000.0f);

    g_OSerial.println("UART Initialized...\n");

    for (;;)
    {
        sleep_mode();

        uint16_t data = serial_get_data<uint16_t>(g_UART_IBuffer);
        if (data == 1337)
        {
            g_OSerial.println("1337\n");
        }
    }
    
    return 0;
}
