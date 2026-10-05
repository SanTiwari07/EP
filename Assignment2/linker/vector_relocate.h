/*
 * vector_relocate.h
 * PIC18F4550 USB HID bootloader vector relocation
 * Used with Microchip C18.
 */

#ifndef VECTOR_RELOCATE_H
#define VECTOR_RELOCATE_H

extern void _startup(void);

#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}

#pragma code
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_ISR(void)
{
}

#pragma code
#pragma code _LOW_INTERRUPT_VECTOR = 0x1018
void low_ISR(void)
{
}

#pragma code

#endif
