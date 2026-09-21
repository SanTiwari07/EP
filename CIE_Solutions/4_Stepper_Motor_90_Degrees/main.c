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
    // Stepper motor sequences
    unsigned char cw_seq[]  = {0x09, 0x0C, 0x06, 0x03}; // Clockwise sequence
    unsigned char ccw_seq[] = {0x03, 0x06, 0x0C, 0x09}; // Anti-clockwise sequence
    
    // 50 steps = 90 degrees (Assuming a standard 1.8 degree/step motor)
    int steps_for_90 = 50; 
    int i; // Loop counter variable
    
    // Set PORTB as output for the motor driver
    TRISB = 0x00;
    PORTB = 0x00;
    
    while(1) { // Infinite loop
        // 1. Rotate 90 degrees Clockwise
        for(i = 0; i < steps_for_90; i++) {
            PORTB = cw_seq[i % 4]; // Send the sequence pattern
            delay_ms(100);         // Wait between steps
        }
        
        delay_ms(1000); // Stop for 1 second
        
        // 2. Rotate 90 degrees Anti-Clockwise
        for(i = 0; i < steps_for_90; i++) {
            PORTB = ccw_seq[i % 4]; // Send the reverse pattern
            delay_ms(100);          // Wait between steps
        }
        
        delay_ms(1000); // Stop for 1 second
    }
}