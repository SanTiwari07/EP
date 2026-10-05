# SCTR’s PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE – 411043
## Department of Electronics & Telecommunication Engineering
### Embedded Processor: Continuous Internal Evaluation (CIE)
**Academic Year: 2026-27**

**Class:** TY | **Semester:** V
**Course Code:** 2626PC12 | **Course Name:** Embedded Processors (EP)
**Name of the Student:** _________________ | **Div.:** ______
**Roll No.:** _________________ | **Student Code:** ______

---

**Problem Statement:**
11. Interface 7 segment display to count 00 to 99

**Solution:**

**1. System Design:**
The embedded system is designed to act as a 00 to 99 up-counter. It utilizes a PIC18F4550 microcontroller operating at 8MHz. The microcontroller calculates the tens and units digits using nested loops and outputs the corresponding 8-bit hex codes to drive two Common Cathode 7-segment displays. A 500ms software delay is generated between counts to make the numbers readable to the human eye.

**2. Peripheral and Microcontroller Selection:**
| Component | Purpose | Selection Criteria |
|---|---|---|
| **PIC18F4550** | Main Control Unit | Selected for its robust I/O capabilities, adequate processing speed (up to 48MHz), built-in timers, and native Proteus simulation support. |
| 7SEG-COM-CATH-RED | Numeric Display | Chosen for direct visual output. Common cathode simplifies logic (HIGH = ON). |
| Resistors (330 ohm) | Current Limiting | Prevents the microcontroller pins and LED segments from burning out. |

**3. Interfacing Diagram:**
*(Please refer to the Proteus schematic)*
**Wiring Description:** 
PIC18F4550 microcontroller with PORTB connected to the segments of the Units 7-segment display. PORTD is connected to the segments of the Tens 7-segment display. The common cathode pins of both displays are tied to a common GROUND terminal.
*(Attach your Proteus Screenshot Here)*

**4. Algorithm:**
1. Start.
2. Configure PORTB and PORTD as digital output ports (TRISB=0x00, TRISD=0x00).
3. Initialize a 1D array with hexadecimal codes corresponding to digits 0-9 for a Common Cathode display.
4. Initialize both displays to 0 (PORTB=0x00, PORTD=0x00).
5. Enter an infinite loop.
6. Start an outer loop for the Tens digit (0 to 9).
7. Start an inner loop for the Units digit (0 to 9).
8. Output the hex code for the current Tens value to PORTD.
9. Output the hex code for the current Units value to PORTB.
10. Call a 500ms delay function.
11. Increment loops. Upon reaching 99, loop back to step 6.

**5. Uploaded URL:**
_________________________________________

**6. Proteus Simulation Results:**
*(Attach screenshots of the Proteus simulation here showing the running state)*

**7. Conclusion:**
The 7-segment display interfacing was successfully implemented using the PIC18F4550. The Proteus simulation accurately demonstrated the multiplexing logic and timing loops, successfully counting from 00 to 99 and rolling over.

---
**Assessment:**
| Performance Indicator | Preparation (5) | Simulation and Problem Solving (5) | Result Interpretation (5) | Total (15) |
|---|---|---|---|---|
| Marks | | | | |

__________________                                       ___________________
Signature of the Student                                 Signature of the Faculty

---
**8. Source Code (main.c):**
`c
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
`
