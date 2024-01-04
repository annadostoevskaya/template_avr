/**
 * File: Serial.cpp
 * Author: github.com/annadostoevskaya
 * Date: 01/04/2024 07:50:05
 * Last Modified Date: 01/04/2024 07:50:05
 */

#include <avr/io.h>
#include "Serial.hpp"

size_t Serial::println(const char* str, size_t n)
{
    size_t i = 0;
    while (i < n)
    {
        write(str[i]);
        i += 1;
    }

    return i;
}

void Serial::write(char c)
{
    while (!(UCSRA & _BV(UDRE)))
        ;;

    TXB = c;
}
