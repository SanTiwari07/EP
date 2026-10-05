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
13. Interface Stepper motor and rotate motor clockwise and anticlockwise wise in 90

**Solution:**

**1. System Design:**
This system precisely controls the angular position of a Unipolar Stepper Motor using a PIC18F4550 microcontroller. Since the MCU cannot supply sufficient current to drive the motor coils directly, a ULN2003A Darlington Transistor array is used as a current amplifier. The software generates a 4-step unipolar driving sequence. Assuming a standard 1.8-degree step angle, the MCU iterates the sequence 50 times to achieve exactly 90 degrees of rotation in both clockwise and anti-clockwise directions.

**2. Peripheral and Microcontroller Selection:**
| Component | Purpose | Selection Criteria |
|---|---|---|
| **PIC18F4550** | Main Control Unit | Selected for its robust I/O capabilities, adequate processing speed (up to 48MHz), built-in timers, and native Proteus simulation support. |
| MOTOR-STEPPER (Unipolar) | Precision Actuator | Allows exact angular positioning, essential for robotics or industrial control. |
| ULN2003A | Motor Driver / Current Amplifier | Required to boost the low-current logic signals from the PIC to high-current coil drives. |

**3. Interfacing Diagram:**
*(Please refer to the Proteus schematic)*
**Wiring Description:** 
PIC18F4550 microcontroller with pins RB0-RB3 connected to inputs IN1-IN4 of a ULN2003A driver. Outputs OUT1-OUT4 of the ULN2003A are connected to the 4 coil pins of the unipolar Stepper Motor. The common pins of the Stepper Motor and the COM pin of the ULN2003A are tied to a POWER terminal.
*(Attach your Proteus Screenshot Here)*

**4. Algorithm:**
1. Start.
2. Configure lower bits of PORTB as output (TRISB=0x00).
3. Define a 4-element array for the Clockwise step sequence (e.g., 0x09, 0x0C, 0x06, 0x03).
4. Define a 4-element array for the Anti-Clockwise step sequence (e.g., 0x03, 0x06, 0x0C, 0x09).
5. Calculate required steps: 90 degrees / 1.8 degrees per step = 50 steps.
6. Enter infinite loop.
7. Loop 50 times: Send CW array values to PORTB sequentially with a 100ms delay between steps.
8. Wait for 1000ms.
9. Loop 50 times: Send CCW array values to PORTB sequentially with a 100ms delay between steps.
10. Wait for 1000ms. Repeat from step 6.

**5. Uploaded URL:**
_________________________________________

**6. Proteus Simulation Results:**
*(Attach screenshots of the Proteus simulation here showing the running state)*

**7. Conclusion:**
The unipolar stepper motor was successfully interfaced and controlled using a PIC18F4550 and ULN2003A driver. The simulation validated the step-sequencing algorithm, resulting in precise 90-degree bidirectional rotations without skipping steps.

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
`
