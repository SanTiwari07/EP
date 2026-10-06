# Continuous Internal Evaluation (CIE 1) Solutions & Comprehensive Viva Guide

**Course:** Embedded Processors Laboratory (EPL) [2625PC32 / 2626PC12]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Overview of CIE 1 Laboratory Evaluation

The Continuous Internal Evaluation 1 (CIE 1) assesses practical hardware interfacing, bare-metal microcontroller firmware development, and electronic circuit simulation skills using **PIC18F4550** and **Proteus 8 Professional / MPLAB C18 / XC8**.

Each project in this directory contains:
- Complete bare-metal **Embedded C firmware** (`main.c`)
- Pre-compiled production artifacts (`.hex`, `.cof`, `.map`)
- Fully wired **Proteus 8 Professional Schematic Simulation** (`.pdsprj`)
- Formal academic submission documentation (`Report.md`)
- Step-by-step Proteus execution instructions (`Proteus_Instructions.txt`)
- In-depth technical documentation with **Graded Viva Voce Question Banks** (`README.md`)

---

## 2. Project Directory Map & Comparison Matrix

| No. | Project Directory | Problem Statement | Core Hardware Peripherals | Primary Concepts Demonstrated |
|:---:|:---|:---|:---|:---|
| **1** | [`1_7_Segment_Display/`](1_7_Segment_Display/README.md) | **Problem 11:** Interface 7-segment display to count 00 to 99 | Dual Common Cathode 7-segment displays, 330 Ω resistor arrays, PORTB & PORTD | BCD/Hex segment encoding, dual-port output driving, software timing loops |
| **2** | [`2_Smart_Traffic_Light/`](2_Smart_Traffic_Light/README.md) | **Problem 5:** Smart Traffic Light Control | Red, Yellow, Green LED indicators, current limiting resistors, PORTB | Finite State Machine (FSM), sequential state transitions, highway timing delay cycles |
| **3** | [`3_Home_Automation/`](3_Home_Automation/README.md) | **Problem 2:** Home Automation using suitable microcontroller | Logic switches (sensors), Relays/LEDs (appliances), PORTB & PORTD | Digital input polling, active-high signal conditioning, appliance isolation |
| **4** | [`4_Stepper_Motor_90_Degrees/`](4_Stepper_Motor_90_Degrees/README.md) | **Problem 13:** Interface Stepper motor and rotate clockwise & anticlockwise by 90° | 4-phase Unipolar Stepper Motor, ULN2003A Darlington Transistor Driver, PORTB | Step angle calculations ($\theta = 1.8^\circ$, 50 steps), 2-phase full-step excitation sequence |

---

## 3. General PIC18F4550 Configuration Directives

All CIE 1 solutions use the following configuration pragmas tailored for Proteus simulation and physical target boards:

```c
#include <p18f4550.h>

// Basic Configuration Pragmas
#pragma config FOSC = HS    // High Speed Crystal Oscillator (External 8-20 MHz)
#pragma config WDT = OFF    // Watchdog Timer Disabled (prevents unexpected resets during delays)
#pragma config LVP = OFF    // Low Voltage ICSP Programming Disabled (frees RB5/PGM pin for digital I/O)
```

### Why these configuration bits matter:
1. **`FOSC = HS` (Oscillator Selection):** Configures the internal oscillator amplifier for high-speed quartz crystals (typically 8 MHz to 20 MHz), ensuring stable clock generation for instruction cycles ($T_{cy} = 4 / F_{osc}$).
2. **`WDT = OFF` (Watchdog Timer):** The Watchdog Timer is an on-chip free-running counter that resets the CPU if not periodically cleared with `ClrWdt()`. Because our simulation projects use long blocking delay routines (e.g., 5 seconds in traffic lights, 500 ms in counters), keeping WDT enabled would trigger periodic CPU resets.
3. **`LVP = OFF` (Low Voltage Programming):** When Low Voltage Programming is enabled, dedicated pin `RB5/PGM` cannot be used as general-purpose digital I/O because any spurious high pulse places the chip into programming mode. Disabling LVP ensures full Port B availability.

---

## 4. Software Delay Mechanics & Calibration

In bare-metal embedded C on PIC18F, simple delays are implemented using nested decrement loops:

```c
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++); // Calibrated loop ~ 1 ms at 8 MHz
    }
}
```

### Mathematical Calibration:
- Oscillator Frequency: $F_{osc} = 8\text{ MHz}$
- Instruction Cycle: $T_{cy} = \frac{4}{F_{osc}} = \frac{4}{8\text{ MHz}} = 0.5\,\mu\text{s}$
- Each iteration of the inner C loop compiles to approximately 10 to 12 instruction cycles in Microchip C18 (including loop decrement, conditional branch, and pipeline refill).
- Number of iterations for $1\text{ ms} = 1000\,\mu\text{s}$:
  $$\text{Iterations} \approx \frac{1000\,\mu\text{s}}{12 \times 0.5\,\mu\text{s}} \approx 165\text{ iterations}$$
- Hence, the inner loop of 165 iterations generates an approximate 1 ms delay, scaled linearly by `ms`.

---

---

## 5. Universal Embedded C Syntax & "Why Did You Write This Line?" Viva Questions

Every practical oral examination begins with line-by-line questions on your C code. Master these foundational answers:

### Q1: Why did you write `#include <p18f4550.h>`? What does it do?
**Answer:**  
It is a C preprocessor directive that imports the device-specific register address map for the PIC18F4550. It maps friendly C identifiers like `TRISB`, `PORTB`, `LATB`, `PORTD` to their actual physical byte addresses in the Special Function Register (SFR) memory space (located between `0xF80` and `0xFFF`). Without it, the compiler cannot recognize any microcontroller peripheral registers.

### Q2: What is `#pragma`? What does `#pragma config` mean?
**Answer:**  
`#pragma` is a standardized C preprocessor directive used to pass target-specific instructions or compiler extensions that standard ANSI C cannot express.  
`#pragma config` writes configuration bytes into the non-volatile Configuration Fuse Registers (located at flash addresses `0x300000` to `0x30000D`). These bits are read by the hardware state machines during Power-on-Reset (POR) to configure the system oscillator, Watchdog Timer, brown-out reset, and code protection before any software instructions execute.

### Q3: Why do you write `while(1)` in every microcontroller program? Can `main()` ever return?
**Answer:**  
In embedded systems, microcontrollers operate as dedicated standalone controllers without an underlying operating system. If `main()` returns (`return 0`), the Program Counter (PC) would advance into unprogrammed flash memory executing undefined opcodes or `0xFFFF`, resulting in CPU crashes or continuous resets. Therefore, `while(1)` forms an infinite loop that keeps the microcontroller running forever from power-on until power is turned off.

### Q4: What does TRIS stand for? Why does `0` configure an Output and `1` configure an Input?
**Answer:**  
`TRIS` stands for **Tri-State**. Microchip established the convention:
- **`0`** looks like **O** for **Output** (enables the low-impedance push-pull output driver transistors).
- **`1`** looks like **I** for **Input** (disables the output driver, placing the pin in a high-impedance state so external signals can safely be sampled).

### Q5: Why is `unsigned char` preferred over `int` for port data and lookup tables?
**Answer:**  
Microcontroller ports on the PIC18F are 8 bits wide ($1\text{ byte}$). An `unsigned char` in C is exactly 8 bits ($0$ to $255$). Using `unsigned char` guarantees an exact 1:1 hardware match, uses minimal RAM, and avoids the overhead of 16-bit integer conversions.

### Q6: What is the difference between `PORTB` and `LATB`? When MUST you use `LATB`?
**Answer:**  
- **`PORTB` (Port Register):** Reading `PORTB` samples the **physical voltage** present on the pins. Writing to `PORTB` writes to the output latch, but executes a **Read-Modify-Write (RMW)** sequence.
- **`LATB` (Latch Register):** Reading or writing `LATB` accesses the internal output flip-flops directly without reading the external pins.
- **When to use `LATB`:** In high-speed switching or bit-manipulation (`LATBbits.LATB0 = 1;`). If a pin has capacitive load and rises slowly, writing to `PORTB` can inadvertently read adjacent pins as `0` and clear them. Writing to `LATB` completely eliminates Read-Modify-Write hazards.

### Q7: In `for(j = 0; j < 165; j++);`, what does the empty semicolon `;` at the end do?
**Answer:**  
The semicolon terminates the `for` statement with an empty body (a null statement). The CPU simply increments `j`, compares it against 165, and loops, effectively burning a predictable number of instruction clock cycles to create a calibrated software time delay.

---

## 6. Master Viva Voce Preparation Roadmap

Examiners conduct practical oral examinations by evaluating four progressive layers of understanding:

```mermaid
flowchart TD
    L1["Level 1: Fundamentals (What & Why)<br>Core concepts, active-high/low, pull-ups, components"] --> L2["Level 2: Direct Implementation<br>Registers TRIS/PORT/LAT, config pragmas, hex codes"]
    L2 --> L3["Level 3: Circuit & System Design<br>Drivers, flyback diodes, multiplexing, debouncing"]
    L3 --> L4["Level 4: Advanced & Troubleshooting<br>RMW hazards, interrupt conversions, EMI protection"]
```

Explore each project's sub-README for dedicated, question-by-question preparation with technically precise answers:
- **[1. 7-Segment Display Counter Viva Questions](1_7_Segment_Display/README.md#6-comprehensive-viva-voce--oral-examination-guide)**
- **[2. Smart Traffic Light Controller Viva Questions](2_Smart_Traffic_Light/README.md#6-comprehensive-viva-voce--oral-examination-guide)**
- **[3. Home Automation Subsystem Viva Questions](3_Home_Automation/README.md#6-comprehensive-viva-voce--oral-examination-guide)**
- **[4. Stepper Motor 90-Degree Rotation Viva Questions](4_Stepper_Motor_90_Degrees/README.md#6-comprehensive-viva-voce--oral-examination-guide)**
