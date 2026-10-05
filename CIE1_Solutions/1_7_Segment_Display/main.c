#include <p18f4550.h>

// Basic Configuration for Proteus Simulation
#pragma config FOSC = HS    // High Speed Crystal Oscillator
#pragma config WDT = OFF    // Watchdog Timer OFF
#pragma config LVP = OFF    // Low Voltage Programming OFF

// Simple delay function (approximate)
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++); // 1ms delay loop
    }
}

void main(void) {
    // Hex codes for Common Cathode 7-segment display (0 to 9)
    unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    
    // Variables must be declared at the top in C18
    int tens, units;
    
    // Set PORTB and PORTD as output pins
    TRISB = 0x00; 
    TRISD = 0x00;
    
    // Initialize displays to 0
    PORTB = 0x00;
    PORTD = 0x00;
    
    while(1) { // Infinite loop
        // Outer loop for Tens digit (0 to 9)
        for(tens = 0; tens < 10; tens++) {
            // Inner loop for Units digit (0 to 9)
            for(units = 0; units < 10; units++) {
                PORTD = seg_code[tens];   // Show Tens digit
                PORTB = seg_code[units];  // Show Units digit
                delay_ms(500);            // Wait for half a second
            }
        }
    }
}