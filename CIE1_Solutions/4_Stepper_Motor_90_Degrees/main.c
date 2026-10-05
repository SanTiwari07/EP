#include <p18f4550.h>

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++);
    }
}

void main(void) {

    unsigned char cw_seq[]  = {0x09, 0x0C, 0x06, 0x03};
    unsigned char ccw_seq[] = {0x03, 0x06, 0x0C, 0x09}; 

    int steps_for_360 = 180; // assuming 1.8 degrees per step
    int i;
    
    TRISB = 0x00;
    PORTB = 0x00;

    while(1) { 
        for(i = 0; i < steps_for_360; i++) {
            PORTB = cw_seq[i % 4]; 
            delay_ms(100);        
        }

        delay_ms(1000);

        for(i = 0; i < steps_for_360; i++) {
            PORTB = ccw_seq[i % 4]; 
            delay_ms(100);        
        }

        delay_ms(1000);
    }
}
