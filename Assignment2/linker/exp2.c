#include <p18f4550.h>
#include "vector_relocate.h"

void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
        for(j = 0; j < 710; j++);
}

void main(void)
{
    ADCON1 = 0x0F;

    INTCON2bits.RBPU = 0;      

    TRISBbits.TRISB0 = 1;      
    TRISBbits.TRISB1 = 1;      

    TRISCbits.TRISC1 = 0;      
    TRISCbits.TRISC2 = 0;       

    TRISD = 0x00;            
    PORTD = 0x00;
    LATCbits.LATC1 = 0;        
    LATCbits.LATC2 = 0;         

    while(1)
    {
        
        if(PORTBbits.RB1 == 0)
        {
            LATCbits.LATC1 = 1;     
            LATCbits.LATC2 = 1;   

            PORTD = 0x01;

            while(PORTD != 0x80)
            {
                delay(250);
                PORTD = PORTD << 1;
            }

            delay(250);
        }

       
        if(PORTBbits.RB0 == 0)
        {
            LATCbits.LATC1 = 0;    
            LATCbits.LATC2 = 0;     

            PORTD = 0x80;

            while(PORTD != 0x01)
            {
                delay(250);
                PORTD = PORTD >> 1;
            }

            delay(250);
        }
    }
}