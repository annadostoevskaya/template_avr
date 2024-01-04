/**
 * File: Serial.hpp
 * Author: github.com/annadostoevskaya
 * Date: 01/04/2024 07:50:05
 * Last Modified Date: 01/04/2024 07:50:05
 */

#pragma once

#include <avr/io.h>

#define __need_size_t
#include <stddef.h>

#if defined(__clang__)
#define FORCEINLINE [[gnu::always_inline]] [[gnu::gnu_inline]] extern inline

#elif defined(__GNUC__)
#define FORCEINLINE __attribute__((always_inline)) inline

#elif defined(_MSC_VER)
#pragma warning(error: 4714)
#define FORCEINLINE __forceinline

#else
#error Unsupported compiler
#endif

class Serial
{
public:
    template<size_t N> 
    FORCEINLINE size_t println(const char (&str)[N]) 
    { 
        return println(str, N); 
    }

    size_t println(const char* str, size_t n);
    void write(char c);
};