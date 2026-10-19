#include <p18f4550.h>
#include "vector_relocate.h"

void todelay(void);

void main(void)
{
    TRISD = 0;

    while (1)
    {
        PORTD = 0x00;
        todelay();
        PORTD = 0xFF;
        todelay();
    }
}

void todelay(void)
{
    T0CON = 0x08;
    TMR0H = 0xFB;
    TMR0L = 0x4C;
    T0CONbits.TMR0ON = 1;
    while (INTCONbits.TMR0IF == 0);
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IF = 0;
}