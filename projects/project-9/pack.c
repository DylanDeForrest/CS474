#include "pack.h"

unsigned int read_u32(void *addr)
{
    unsigned char *p = addr;
    return ((unsigned int)p[0] << 24) |
           ((unsigned int)p[1] << 16) |
           ((unsigned int)p[2] << 8)  |
           (unsigned int)p[3];
}

unsigned short read_u16(void *addr)
{
    unsigned char *p = addr;
    return (unsigned short)(((unsigned short)p[0] << 8) | p[1]);
}

unsigned char read_u8(void *addr)
{
    unsigned char *p = addr;
    return p[0];
}

void write_u32(void *addr, unsigned long value)
{
    unsigned char *p = addr;
    p[0] = (value >> 24) & 0xFF;
    p[1] = (value >> 16) & 0xFF;
    p[2] = (value >> 8)  & 0xFF;
    p[3] = value & 0xFF;
}

void write_u16(void *addr, unsigned int value)
{
    unsigned char *p = addr;
    p[0] = (value >> 8) & 0xFF;
    p[1] = value & 0xFF;
}

void write_u8(void *addr, unsigned char value)
{
    unsigned char *p = addr;
    p[0] = value;
}
