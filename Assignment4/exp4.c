#include <p18f4550.h>
#include "vector_relocate.h"

void MSdelay(unsigned int t)
{
    unsigned int i, j;
    for (i = 0; i < t; i++)
    {
        for (j = 0; j < 1275; j++);
    }
}

void main(void)
{
    TRISCbits.TRISC2 = 0;
    TRISD = 0;
    PR2 = 0xBA;
    TMR2 = 0x00;
    CCP1CON = 0x0C;
    T2CON = 0x07;

    PORTDbits.RD5 = 1;
    PORTDbits.RD6 = 0;

    while (1)
    {
        CCPR1L = 0x25;
        CCP1CONbits.DC1B0 = 0;
        CCP1CONbits.DC1B1 = 1;
        MSdelay(2000);

        CCPR1L = 0x4B;
        CCP1CONbits.DC1B0 = 0;
        CCP1CONbits.DC1B1 = 0;
        MSdelay(2000);

        CCPR1L = 0x25;
        CCP1CONbits.DC1B0 = 0;
        CCP1CONbits.DC1B1 = 1;
        MSdelay(2000);

        CCPR1L = 0x94;
        CCP1CONbits.DC1B0 = 0;
        CCP1CONbits.DC1B1 = 1;
        MSdelay(2000);
    }
}
