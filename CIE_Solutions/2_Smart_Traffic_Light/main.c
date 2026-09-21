#include <p18f4550.h>

// Basic Configuration
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

// Simple delay function
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++);
    }
}

void main(void) {
    // Configure PORTB as output for LEDs
    TRISB = 0x00;
    PORTB = 0x00; // Turn off all LEDs initially
    
    while(1) { // Infinite loop
        // Step 1: Red Light ON (Pin RB0)
        PORTB = 0x01; // 0000 0001 in binary
        delay_ms(5000); // Wait 5 seconds
        
        // Step 2: Green Light ON (Pin RB2)
        PORTB = 0x04; // 0000 0100 in binary
        delay_ms(5000); // Wait 5 seconds
        
        // Step 3: Yellow Light ON (Pin RB1)
        PORTB = 0x02; // 0000 0010 in binary
        delay_ms(2000); // Wait 2 seconds
    }
}