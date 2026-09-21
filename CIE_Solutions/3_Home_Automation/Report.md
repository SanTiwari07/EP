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
2. Home Automation Using suitable microcontroller

**Solution:**

**1. System Design:**
The system demonstrates basic home automation by reading environmental or user inputs and driving high-power appliances accordingly. A PIC18F4550 microcontroller is used as the central hub. PORTB is configured as an input port to read the state of logic switches (simulating wall switches or sensors). Based on these inputs, PORTD is manipulated to drive output pins, which in a real scenario would activate relays to switch AC appliances like fans or lights.

**2. Peripheral and Microcontroller Selection:**
| Component | Purpose | Selection Criteria |
|---|---|---|
| **PIC18F4550** | Main Control Unit | Selected for its robust I/O capabilities, adequate processing speed (up to 48MHz), built-in timers, and native Proteus simulation support. |
| LOGICSTATE | Input Simulation | Used in Proteus to simulate digital high (5V) or low (0V) signals from a sensor/switch. |
| LED-BLUE / LED-GREEN | Appliance Simulation | Used to visually represent home appliances (e.g., Light, Fan) turning ON/OFF. |

**3. Interfacing Diagram:**
*(Please refer to the Proteus schematic)*
**Wiring Description:** 
PIC18F4550 microcontroller with two LOGICSTATE blocks connected to input pins RB0 and RB1. Two LEDs are connected to output pins RD0 and RD1, with their cathodes tied to a common GROUND terminal.
*(Attach your Proteus Screenshot Here)*

**4. Algorithm:**
1. Start.
2. Configure PORTB as an input port (TRISB=0xFF).
3. Configure PORTD as an output port (TRISD=0x00).
4. Initialize PORTD to 0x00 (all appliances OFF).
5. Enter an infinite loop.
6. Read the digital state of pin RB0.
7. If RB0 is HIGH, set RD0 HIGH (Turn ON Appliance 1). Else, set RD0 LOW.
8. Read the digital state of pin RB1.
9. If RB1 is HIGH, set RD1 HIGH (Turn ON Appliance 2). Else, set RD1 LOW.
10. Repeat from step 6 continuously.

**5. Uploaded URL:**
_________________________________________

**6. Proteus Simulation Results:**
*(Attach screenshots of the Proteus simulation here showing the running state)*

**7. Conclusion:**
The foundation for a home automation system was successfully established. The simulation proved the microcontroller can continuously poll digital inputs and safely trigger corresponding isolated outputs.

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
`
