import os

base_dir = r'C:\Users\sansk\Desktop\EP CIE\CIE_Solutions'
os.makedirs(base_dir, exist_ok=True)

problems = [
    {
        'folder': '1_7_Segment_Display',
        'ps': 'Interface 7 segment display to count 00 to 99',
        'sys_design': 'The system uses a PIC18F4550 microcontroller to drive two 7-segment displays (Common Cathode) representing a two-digit counter from 00 to 99. The units digit is connected to PORTB and the tens digit is connected to PORTD. The microcontroller runs a loop updating the digits with a specific delay.',
        'peripherals': '| 7-Segment Display (CC) | Display numeric values | Two required for 00 to 99 counting. Easy to interface. |\n| Resistors (330 ohm) | Current limiting | Protect the 7-segment LEDs from overcurrent. |',
        'diagram_desc': 'PIC18F4550 with PORTB connected to the first 7-segment display (Units) via 330-ohm resistors, and PORTD connected to the second 7-segment display (Tens) via 330-ohm resistors. Crystal oscillator connected to OSC1 and OSC2.',
        'algo': '1. Start.\n2. Configure PORTB and PORTD as output ports.\n3. Initialize a 1D array with hex codes for digits 0-9 for a Common Cathode display.\n4. Use a nested loop: outer loop for Tens (0 to 9), inner loop for Units (0 to 9).\n5. Output the corresponding hex code for the Tens digit on PORTD.\n6. Output the corresponding hex code for the Units digit on PORTB.\n7. Wait for a specified delay.\n8. Repeat from step 4 continuously.',
        'conclusion': 'The 7-segment display interfacing with PIC18F4550 was successfully designed and simulated. The system counts accurately from 00 to 99 and rolls back to 00, demonstrating basic I/O port manipulation and delay generation.',
        'code': '''#include <xc.h>
#define _XTAL_FREQ 8000000

// Configuration bits (Standard for PIC18F4550 Proteus Simulation)
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main() {
    // Hex codes for Common Cathode 7-segment display (0-9)
    unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    
    TRISB = 0x00; // Set PORTB as Output (Units)
    TRISD = 0x00; // Set PORTD as Output (Tens)
    PORTB = 0x00;
    PORTD = 0x00;
    
    while(1) {
        for(int tens = 0; tens < 10; tens++) {
            for(int units = 0; units < 10; units++) {
                PORTD = seg_code[tens]; // Display Tens digit
                PORTB = seg_code[units]; // Display Units digit
                __delay_ms(500); // 500ms delay between counts
            }
        }
    }
}'''
    },
    {
        'folder': '2_Smart_Traffic_Light',
        'ps': 'Smart Traffic Light Control',
        'sys_design': 'The system simulates a smart traffic light control mechanism at an intersection. Three LEDs (Red, Yellow, Green) are connected to PORTB of the PIC18F4550 to represent a traffic light. The controller switches the lights in a standard sequence (Red -> Green -> Yellow -> Red) with predefined delays.',
        'peripherals': '| LEDs (Red, Yellow, Green) | Traffic light indication | Low power consumption, easy I/O mapping. |\n| Resistors (330 ohm) | Current limiting | Protect LEDs from high current. |',
        'diagram_desc': 'PIC18F4550 with RB0 connected to a Red LED, RB1 connected to a Yellow LED, and RB2 connected to a Green LED through 330-ohm series resistors. Cathodes of all LEDs grounded.',
        'algo': '1. Start.\n2. Configure PORTB (specifically RB0, RB1, RB2) as output.\n3. Turn ON Red LED (RB0=1), others OFF. Delay for 5 seconds.\n4. Turn ON Green LED (RB2=1), others OFF. Delay for 5 seconds.\n5. Turn ON Yellow LED (RB1=1), others OFF. Delay for 2 seconds.\n6. Repeat from step 3 continuously.',
        'conclusion': 'A basic Smart Traffic Light control system was implemented using PIC18F4550. The precise time delays and proper output pin sequencing successfully simulated a traffic light intersection.',
        'code': '''#include <xc.h>
#define _XTAL_FREQ 8000000

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main() {
    TRISB = 0x00; // Set PORTB as Output
    PORTB = 0x00;
    
    while(1) {
        // Red Light
        PORTB = 0x01; // RB0 = 1 (Red)
        __delay_ms(5000);
        
        // Green Light
        PORTB = 0x04; // RB2 = 1 (Green)
        __delay_ms(5000);
        
        // Yellow Light
        PORTB = 0x02; // RB1 = 1 (Yellow)
        __delay_ms(2000);
    }
}'''
    },
    {
        'folder': '3_Home_Automation',
        'ps': 'Home Automation Using Suitable Microcontroller',
        'sys_design': 'The system uses a PIC18F4550 microcontroller to automate home appliances. Input switches (simulating sensors or user commands) are connected to PORTB. Based on the switch states, the microcontroller drives output relays connected to PORTD, which turn home appliances (like a Fan or Light) ON or OFF.',
        'peripherals': '| SPST Switches/Buttons | User input / Sensor simulation | Simulates human interaction or environmental sensors. |\n| Relays (5V) & Bulbs | Appliance Control | Required to switch high-voltage loads from low-voltage MCU. |\n| Relay Driver (ULN2003 or NPN) | Driving Relays | MCU pins cannot provide enough current for relay coils. |',
        'diagram_desc': 'PIC18F4550 with switches connected to RB0 and RB1 (pulled low by default). RD0 and RD1 are connected to the base of NPN transistors (or a ULN2003 driver) to switch 5V Relays. Relays control an AC Bulb and a DC Motor (Fan).',
        'algo': '1. Start.\n2. Configure PORTB as input (for switches/sensors).\n3. Configure PORTD as output (for relays).\n4. Read the state of switch 1 on RB0.\n5. If RB0 is HIGH, set RD0 HIGH (turn ON Light), else set RD0 LOW.\n6. Read the state of switch 2 on RB1.\n7. If RB1 is HIGH, set RD1 HIGH (turn ON Fan), else set RD1 LOW.\n8. Repeat from step 4 continuously.',
        'conclusion': 'The home automation system was successfully designed using the PIC18F4550. It accurately reads digital inputs and controls isolated output loads via relays, proving its feasibility for smart home integration.',
        'code': '''#include <xc.h>
#define _XTAL_FREQ 8000000

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main() {
    TRISB = 0xFF; // Set PORTB as Input (Switches/Sensors)
    TRISD = 0x00; // Set PORTD as Output (Relays)
    PORTD = 0x00; // Initial state OFF
    
    while(1) {
        // Control Light based on Switch 1
        if(PORTBbits.RB0 == 1) {
            PORTDbits.RD0 = 1; // Turn ON Light Relay
        } else {
            PORTDbits.RD0 = 0; // Turn OFF Light Relay
        }
        
        // Control Fan based on Switch 2
        if(PORTBbits.RB1 == 1) {
            PORTDbits.RD1 = 1; // Turn ON Fan Relay
        } else {
            PORTDbits.RD1 = 0; // Turn OFF Fan Relay
        }
    }
}'''
    },
    {
        'folder': '4_Stepper_Motor_90_Degrees',
        'ps': 'Interface Stepper motor and rotate motor clockwise and anticlockwise wise in 90',
        'sys_design': 'The system interfaces a Unipolar Stepper Motor with a PIC18F4550 microcontroller. The microcontroller generates a precise 4-step sequence on PORTB to rotate the motor. Assuming a standard 1.8-degree step angle motor, it takes 50 steps to complete a 90-degree rotation. The system rotates the motor 90 degrees clockwise, pauses, and then 90 degrees anti-clockwise.',
        'peripherals': '| Stepper Motor (Unipolar) | Precision motion control | Allows precise angular rotation (e.g., 90 degrees). |\n| ULN2003 Motor Driver | Current Amplification | Microcontroller cannot provide the high current needed for motor coils. |',
        'diagram_desc': 'PIC18F4550 with RB0, RB1, RB2, and RB3 connected to the IN1, IN2, IN3, and IN4 pins of a ULN2003 driver. The OUT pins of the ULN2003 are connected to the coils of a 6-wire or 5-wire unipolar stepper motor.',
        'algo': '1. Start.\n2. Configure PORTB (lower 4 bits) as output.\n3. Define a sequence array for Clockwise rotation (e.g., 0x09, 0x0C, 0x06, 0x03).\n4. Define a sequence array for Anti-Clockwise rotation (reverse of CW).\n5. Calculate steps needed for 90 degrees (e.g., 50 steps for a 1.8 deg/step motor).\n6. Loop 50 times passing the CW sequence to PORTB with a small delay between steps.\n7. Wait for 1 second.\n8. Loop 50 times passing the CCW sequence to PORTB with a small delay.\n9. Wait for 1 second and repeat.',
        'conclusion': 'The stepper motor was successfully interfaced with the PIC18F4550 using a ULN2003 driver. The simulation correctly demonstrates the generation of stepping sequences resulting in precise 90-degree rotations in both clockwise and anti-clockwise directions.',
        'code': '''#include <xc.h>
#define _XTAL_FREQ 8000000

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

void main() {
    TRISB = 0x00; // Set PORTB as Output
    PORTB = 0x00;
    
    // Full step sequences
    unsigned char cw_seq[] = {0x09, 0x0C, 0x06, 0x03};
    unsigned char ccw_seq[] = {0x03, 0x06, 0x0C, 0x09};
    
    // Assuming a motor with 1.8 degrees per step (200 steps per revolution)
    // 360 degrees = 200 steps, therefore 90 degrees = 50 steps
    int steps_for_90 = 50; 
    
    while(1) {
        // Rotate 90 degrees Clockwise
        for(int i = 0; i < steps_for_90; i++) {
            PORTB = cw_seq[i % 4];
            __delay_ms(100); // 100ms delay between steps
        }
        
        __delay_ms(1000); // Wait 1 second
        
        // Rotate 90 degrees Anti-Clockwise
        for(int i = 0; i < steps_for_90; i++) {
            PORTB = ccw_seq[i % 4];
            __delay_ms(100); 
        }
        
        __delay_ms(1000); // Wait 1 second
    }
}'''
    }
]

template = '''# SCTR’s PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE – 411043
## Department of Electronics & Telecommunication Engineering
### Embedded Processor: Continuous Internal Evaluation (CIE)
**Academic Year: 2026-27**

**Class:** TY | **Semester:** V
**Course Code:** 2626PC12 | **Course Name:** Embedded Processors (EP)
**Name of the Student:** _________________ | **Div.:** ______
**Roll No.:** _________________ | **Student Code:** ______

---

**Problem Statement:**
{ps}

**Solution:**

**1. System Design:**
{sys_design}

**2. Peripheral and Microcontroller Selection:**
| Component | Purpose | Selection Criteria |
|---|---|---|
| PIC18F4550 | Main Control Unit | Requires adequate I/O ports, timers, and processing speed. |
{peripherals}

**3. Interfacing Diagram:**
*(Please refer to the Proteus schematic. Components: {diagram_desc})*
[Space for Diagram Screenshot]

**4. Algorithm:**
{algo}

**5. Uploaded URL:**
_________________________________________

**6. Proteus Simulation Results:**
*(Attach screenshots of the Proteus simulation here showing the running state)*

**7. Conclusion:**
{conclusion}

**Assessment:**
| Performance Indicator | Preparation (5) | Simulation and Problem Solving (5) | Result Interpretation (5) | Total (15) |
|---|---|---|---|---|
| Marks | | | | |

__________________                                       ___________________
Signature of the Student                                 Signature of the Faculty
'''

proteus_instructions = '''PROTEUS 8 PROFESSIONAL SIMULATION SETUP
---------------------------------------

To run this simulation in Proteus 8 Professional, follow these steps:

1. Open Proteus 8 Professional and click on 'New Project'.
2. Proceed through the wizard, selecting 'DEFAULT' schematic size.
3. If prompted to create a PCB, choose 'Do not create a PCB layout'.
4. If prompted to create Firmware, choose 'No Firmware Project' (we will attach the hex file manually).
5. In the schematic capture window, press 'P' to pick devices.
6. Search and add the following parts:
   - PIC18F4550
   {components}
7. Place the PIC18F4550 on the workspace.
8. Wire the components as per the 'Interfacing Diagram' description in the Report.md file.
9. To program the PIC in Proteus:
   - You need to compile the provided `main.c` file using MPLAB X IDE with XC8 compiler to generate a `.hex` file.
   - Double-click the PIC18F4550 in Proteus.
   - Under 'Program File', browse and select the generated `.hex` file.
   - Set 'Processor Clock Frequency' to 8MHz.
10. Click the 'Play' button at the bottom left to start the simulation.
'''

for p in problems:
    folder_path = os.path.join(base_dir, p['folder'])
    os.makedirs(folder_path, exist_ok=True)
    
    # Write Report
    report_content = template.format(**p)
    with open(os.path.join(folder_path, 'Report.md'), 'w', encoding='utf-8') as f:
        f.write(report_content)
        
    # Write Code
    with open(os.path.join(folder_path, 'main.c'), 'w', encoding='utf-8') as f:
        f.write(p['code'])
        
    # Write Proteus Instructions
    comps = ''
    if '7_Segment' in p['folder']:
        comps = "- 7SEG-COM-CATHODE (7-segment display)\n   - RES (Resistors, set to 330 ohms)"
    elif 'Traffic' in p['folder']:
        comps = "- LED-RED, LED-YELLOW, LED-GREEN\n   - RES (Resistors, set to 330 ohms)"
    elif 'Home_Auto' in p['folder']:
        comps = "- LOGICSTATE (or BUTTON + Resistors for input)\n   - RELAY\n   - NPN Transistor (e.g., 2N2222) or ULN2003\n   - LAMP (or MOTOR) to simulate appliance"
    elif 'Stepper' in p['folder']:
        comps = "- MOTOR-STEPPER (Unipolar)\n   - ULN2003 (Motor Driver)"
    
    ins_content = proteus_instructions.replace('{components}', comps)
    with open(os.path.join(folder_path, 'Proteus_Instructions.txt'), 'w', encoding='utf-8') as f:
        f.write(ins_content)

print('Successfully generated all solutions in', base_dir)
