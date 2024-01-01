/**
 * File: RingBuffer.h
 * Author: github.com/annadostoevskaya
 * Date: 01/01/2024 07:50:05
 * Last Modified Date: 01/01/2024 07:50:07
 */
#pragma once
#define __need_size_t
#include <stddef.h>
#include <stdint.h>

template <size_t Sz = 32>
class RingBuffer
{
private:
    uint8_t m_Buffer[Sz];
    volatile size_t m_Head; 
    volatile size_t m_Tail;
    
    constexpr uint16_t clp2(uint16_t x) // NOTE(annad): Power of two closest to x from above
    {
        uint16_t p2 = 1;
        for (;;)
        {
            if (p2 >= x) return p2;
            p2 <<= 1;
        }

        return 0;
    }

public:
    RingBuffer() : m_Head(0), m_Tail(0) {}
    bool available() { return m_Head != m_Tail; }

    uint8_t read()
    {
        if (m_Tail == m_Head) 
            return 0;

        uint8_t result = m_Buffer[m_Tail];
        m_Tail = (m_Tail + 1) & (clp2(Sz) - 1);

        return result;
    }

    void write(uint8_t data)
    {
        if (m_Head >= Sz)
            m_Head = 0;

        m_Buffer[m_Head] = data;
        m_Head = (m_Head + 1) & (clp2(Sz) - 1);
    }
};
