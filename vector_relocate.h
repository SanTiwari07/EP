#ifndef __vector_relocate_H
#define __vector_relocate_H

/* The following lines perform interrupt vector relocation to work with the USB bootloader. */

extern void _startup(void);

#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_ISR(void);
#pragma interrupt high_ISR
void high_ISR(void)
{
    // High priority ISR code here (empty if none)
}
#pragma code

#pragma code _LOW_INTERRUPT_VECTOR = 0x1018
void low_ISR(void);
#pragma interruptlow low_ISR
void low_ISR(void)
{
    // Low priority ISR code here (empty if none)
}
#pragma code

#endif
