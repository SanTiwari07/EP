#include <p18f4550.h>

// Basic Configuration
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main(void) {
    // Configure PORTB as Input (for Switches/Sensors)
    TRISB = 0xFF; // 1111 1111 (All pins are inputs)
    
    // Configure PORTD as Output (for Relays/Appliances)
    TRISD = 0x00; // 0000 0000 (All pins are outputs)
    
    // Initially turn off all appliances
    PORTD = 0x00;
    
    while(1) { // Infinite loop
        // Read Switch 1 on Pin RB0
        if(PORTBbits.RB0 == 1) {
            PORTDbits.RD0 = 1; // Turn ON Appliance 1 (Light)
        } else {
            PORTDbits.RD0 = 0; // Turn OFF Appliance 1 (Light)
        }
        
        // Read Switch 2 on Pin RB1
        if(PORTBbits.RB1 == 1) {
            PORTDbits.RD1 = 1; // Turn ON Appliance 2 (Fan)
        } else {
            PORTDbits.RD1 = 0; // Turn OFF Appliance 2 (Fan)
        }
    }
}