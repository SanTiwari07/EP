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
5. Smart Traffic Light Control

**Solution:**

**1. System Design:**
The system simulates a smart traffic light control sequence at an intersection using a PIC18F4550 microcontroller. Three distinct LEDs (Red, Yellow, Green) are driven by PORTB pins. The microcontroller is programmed to cycle through a standard traffic light sequence: Red (Stop) for 5 seconds, Green (Go) for 5 seconds, and Yellow (Caution) for 2 seconds. The software utilizes precise delay loops to manage state transitions autonomously.

**2. Peripheral and Microcontroller Selection:**
| Component | Purpose | Selection Criteria |
|---|---|---|
| **PIC18F4550** | Main Control Unit | Selected for its robust I/O capabilities, adequate processing speed (up to 48MHz), built-in timers, and native Proteus simulation support. |
| LED-RED, LED-YELLOW, LED-GREEN | Traffic Light Simulation | Low power, distinct color indications representing standard traffic signals. |
| Resistors (330 ohm) | Current Limiting | Protects LEDs from overcurrent from the 5V MCU pins. |

**3. Interfacing Diagram:**
*(Please refer to the Proteus schematic)*
**Wiring Description:** 
PIC18F4550 microcontroller with pin RB0 connected to the Anode of a Red LED. Pin RB1 is connected to the Anode of a Yellow LED. Pin RB2 is connected to the Anode of a Green LED. The Cathodes of all three LEDs are connected to a common GROUND terminal.
*(Attach your Proteus Screenshot Here)*

**4. Algorithm:**
1. Start.
2. Configure PORTB pins RB0, RB1, and RB2 as digital output ports.
3. Initialize PORTB to 0x00 to turn off all LEDs.
4. Enter an infinite loop.
5. Turn ON Red LED (RB0=1), turn off others. Call 5000ms delay.
6. Turn ON Green LED (RB2=1), turn off others. Call 5000ms delay.
7. Turn ON Yellow LED (RB1=1), turn off others. Call 2000ms delay.
8. Repeat from step 4.

**5. Uploaded URL:**
_________________________________________

**6. Proteus Simulation Results:**
*(Attach screenshots of the Proteus simulation here showing the running state)*

**7. Conclusion:**
A functional smart traffic light control system was successfully simulated. The PIC18F4550 successfully managed multiple digital outputs and executed precise timing delays to accurately mimic a real-world traffic sequence.

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
`
