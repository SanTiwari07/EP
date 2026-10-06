# CIE 1 - Problem 11: Two-Digit 7-Segment Display Up-Counter (00 to 99)

**Course:** Embedded Processors Laboratory (EPL) [2625PC32 / 2626PC12]  
**Academic Year:** 2026–27 | **Class:** T.Y. B.Tech (E&TC)  
**Target Device:** Microchip PIC18F4550 (DIP-40)  
**Development Tools:** MPLAB IDE / MPLAB C18 v3.47 / XC8, Proteus 8 Professional  

---

## 1. Problem Statement
**Problem Statement No. 11:**  
*Interface a 7-segment display to count from 00 to 99.*

The system must continuously increment a two-digit decimal count from `00` up to `99`, rolling over smoothly back to `00`. Each count must remain visible for approximately 500 ms so that the progression is clearly readable to the human eye.

---

## 2. Hardware Architecture & Interfacing Theory

### 2.1 Common Cathode vs. Common Anode
A 7-segment display consists of eight individual LEDs arranged in a figure-8 pattern with a decimal point: labeled **a, b, c, d, e, f, g**, and **dp**.

```text
       --- a ---
      |         |
      f         b
      |         |
       --- g ---
      |         |
      e         c
      |         |
       --- d ---   [dp]
```

- **Common Cathode (CC):** All the LED cathodes (negative terminals) are tied together to system **GND**. An individual segment turns **ON** when its corresponding anode is driven to logic **HIGH (+5V)**.
- **Common Anode (CA):** All the LED anodes are tied together to **+5V (VCC)**. An individual segment turns **ON** when its cathode is driven to logic **LOW (0V)**.

> [!NOTE]
> This project uses **Common Cathode displays** (`7SEG-COM-CATH-RED` in Proteus). Driving a pin HIGH activates the segment.

### 2.2 Segment Encoding Truth Table (Active-High Common Cathode)

Mapping: Bit 7 = `dp`, Bit 6 = `g`, Bit 5 = `f`, Bit 4 = `e`, Bit 3 = `d`, Bit 2 = `c`, Bit 1 = `b`, Bit 0 = `a`.

| Digit | dp (Bit 7) | g (Bit 6) | f (Bit 5) | e (Bit 4) | d (Bit 3) | c (Bit 2) | b (Bit 1) | a (Bit 0) | Binary Byte | Hex Code |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **0** | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | `0011 1111` | **`0x3F`** |
| **1** | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | `0000 0110` | **`0x06`** |
| **2** | 0 | 1 | 0 | 1 | 1 | 0 | 1 | 1 | `0101 1011` | **`0x5B`** |
| **3** | 0 | 1 | 0 | 0 | 1 | 1 | 1 | 1 | `0100 1111` | **`0x4F`** |
| **4** | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | `0110 0110` | **`0x66`** |
| **5** | 0 | 1 | 1 | 0 | 1 | 1 | 0 | 1 | `0110 1101` | **`0x6D`** |
| **6** | 0 | 1 | 1 | 1 | 1 | 1 | 0 | 1 | `0111 1101` | **`0x7D`** |
| **7** | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 | `0000 0111` | **`0x07`** |
| **8** | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | `0111 1111` | **`0x7F`** |
| **9** | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 | `0110 1111` | **`0x6F`** |

### 2.3 Resistor Sizing Calculation
Standard red 7-segment LEDs typically have a forward voltage drop $V_F \approx 1.8\text{V}$ to $2.0\text{V}$ and a nominal operating current $I_F \approx 10\text{ mA}$.
$$R = \frac{V_{CC} - V_F}{I_F} = \frac{5.0\text{V} - 1.8\text{V}}{10\text{ mA}} = \frac{3.2\text{V}}{0.01\text{A}} = 320\,\Omega \implies \text{Standard } 330\,\Omega$$

Each segment line incorporates a $330\,\Omega$ current-limiting resistor to protect the microcontroller output buffers from overcurrent.

---

## 3. Hardware Interfacing & Pinout Map

In this simulation design, direct port-per-display driving is utilized for maximum simplicity:
- **PORTD (RD0 to RD7):** Drives the **Tens Display** (Left digit)
- **PORTB (RB0 to RB7):** Drives the **Units Display** (Right digit)

| PIC18F4550 Pin | Port Pin | Direction | Display | Segment Pin | Function |
|:---:|:---:|:---:|:---:|:---:|:---|
| Pin 19 | RD0 | Output | Tens Display | Segment `a` | Upper horizontal segment |
| Pin 20 | RD1 | Output | Tens Display | Segment `b` | Upper right vertical segment |
| Pin 21 | RD2 | Output | Tens Display | Segment `c` | Lower right vertical segment |
| Pin 22 | RD3 | Output | Tens Display | Segment `d` | Bottom horizontal segment |
| Pin 27 | RD4 | Output | Tens Display | Segment `e` | Lower left vertical segment |
| Pin 28 | RD5 | Output | Tens Display | Segment `f` | Upper left vertical segment |
| Pin 29 | RD6 | Output | Tens Display | Segment `g` | Middle horizontal segment |
| Pin 30 | RD7 | Output | Tens Display | Segment `dp` | Decimal point (kept 0) |
| Pin 33 | RB0 | Output | Units Display | Segment `a` | Upper horizontal segment |
| Pin 34 | RB1 | Output | Units Display | Segment `b` | Upper right vertical segment |
| Pin 35 | RB2 | Output | Units Display | Segment `c` | Lower right vertical segment |
| Pin 36 | RB3 | Output | Units Display | Segment `d` | Bottom horizontal segment |
| Pin 37 | RB4 | Output | Units Display | Segment `e` | Lower left vertical segment |
| Pin 38 | RB5 | Output | Units Display | Segment `f` | Upper left vertical segment |
| Pin 39 | RB6 | Output | Units Display | Segment `g` | Middle horizontal segment |
| Pin 40 | RB7 | Output | Units Display | Segment `dp` | Decimal point (kept 0) |
| Common Pins | CC | — | Both Displays | Cathode Pins | Connected to 0V Common Ground |

---

## 4. Firmware Code & Detailed Annotation

```c
#include <p18f4550.h>

// Microcontroller Configuration Pragmas
#pragma config FOSC = HS    // High Speed Crystal Oscillator
#pragma config WDT = OFF    // Watchdog Timer Disabled
#pragma config LVP = OFF    // Low-Voltage Programming Disabled

// Software delay loop calibrated for ~1 ms per count at 8 MHz
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 165; j++); // 165 inner iterations ~ 1 ms
    }
}

void main(void) {
    // Array of 7-segment hex codes for digits 0 to 9 (Common Cathode)
    unsigned char seg_code[] = {
        0x3F, // '0'
        0x06, // '1'
        0x5B, // '2'
        0x4F, // '3'
        0x66, // '4'
        0x6D, // '5'
        0x7D, // '6'
        0x07, // '7'
        0x7F, // '8'
        0x6F  // '9'
    };
    
    // In Microchip C18 (ANSI C89), variables must be declared at the block beginning
    int tens, units;
    
    // Configure all pins of PORTB and PORTD as Digital Outputs
    TRISB = 0x00; 
    TRISD = 0x00;
    
    // Clear displays initially (all segments OFF)
    PORTB = 0x00;
    PORTD = 0x00;
    
    while(1) { // Infinite counting cycle
        // Outer loop increments the Tens digit (0 to 9)
        for(tens = 0; tens < 10; tens++) {
            // Inner loop increments the Units digit (0 to 9)
            for(units = 0; units < 10; units++) {
                PORTD = seg_code[tens];   // Output tens segment pattern
                PORTB = seg_code[units];  // Output units segment pattern
                delay_ms(500);            // 500 ms human-readable display delay
            }
        }
    }
}
```

---

## 5. Proteus Simulation Setup Instructions

1. Launch **Proteus 8 Professional** and open `7_Segment_Counter.pdsprj`.
2. To assemble or modify the schematic from scratch:
   - Pick devices: `PIC18F4550`, `7SEG-COM-CATH-RED`, `RES` ($330\,\Omega$).
   - Connect `PORTD` pins through $330\,\Omega$ resistors to the Tens display segments.
   - Connect `PORTB` pins through $330\,\Omega$ resistors to the Units display segments.
   - Wire the common cathode pins of both displays to **GROUND**.
3. Double-click the **PIC18F4550** component in Proteus:
   - Set **Processor Clock Frequency** to `8MHz`.
   - Set **Program File** by browsing to `Q1.hex` (or `main.hex`).
4. Click the **Play** button (bottom left) to execute the simulation.
5. Verify that the displays start at `00`, increment to `01, 02, ... 99`, and roll over to `00`.

---

## 6. Comprehensive Viva Voce & Oral Examination Guide

### Level 0: Fundamental Line-by-Line Code Breakdown ("What Does This Line Mean & Why Did You Write It?")

#### Q0.1: Why did you write `#include <p18f4550.h>`? What does it do, and what happens if you omit it?
**Answer:**  
- **What it means:** It is a preprocessor directive that instructs the compiler to include the device-specific header file for the PIC18F4550 microcontroller.
- **Why we wrote it:** This header file contains the memory-mapped definitions for all Special Function Registers (SFRs) like `TRISB`, `PORTB`, `LATB`, `TRISD`, `PORTD`, and their individual bit structures (e.g., `PORTBbits.RB0`).
- **If omitted:** The compiler will throw fatal errors because names like `TRISB` or `PORTD` will be completely unrecognized identifiers.

#### Q0.2: What is `#pragma`? What does `#pragma config FOSC = HS` mean? What does `HS` stand for?
**Answer:**  
- **What it means:** `#pragma` is a directive used to issue compiler-specific or target-specific commands that cannot be expressed in standard ANSI C. `#pragma config` specifically writes values into the non-volatile Configuration Fuse Registers (Configuration Words) located at program memory address `0x300000` to `0x30000D`.
- **Why we wrote it:** `FOSC = HS` selects the **High-Speed Crystal Oscillator** mode.
- **What `HS` stands for:** *High Speed*. It tells the on-chip oscillator inverter circuitry to operate with external quartz crystals or ceramic resonators in the medium-to-high frequency range ($4\text{ MHz}$ to $25\text{ MHz}$). At $8\text{ MHz}$, `HS` mode provides the clock driving all instruction cycles.

#### Q0.3: Why did you write `#pragma config WDT = OFF`? What is the Watchdog Timer, and what happens if you leave it ON?
**Answer:**  
- **What it means:** Disables the internal hardware **Watchdog Timer (WDT)**.
- **Why we wrote it:** The Watchdog Timer is an on-chip free-running counter clocked by an independent RC oscillator. In commercial systems, it is designed to recover from firmware crashes: if the software does not periodically execute `ClrWdt()` before the timer overflows (typically every $4\text{ ms}$ to $131\text{ ms}$), the WDT forces an automatic hardware CPU reset.
- **If left ON (`WDT = ON`):** Because our program uses long blocking delays (`delay_ms(500)`), the watchdog timer will overflow midway through the delay, resetting the microcontroller repeatedly every few milliseconds. The counter will never increment past `00` or `01`.

#### Q0.4: Why did you write `#pragma config LVP = OFF`? What is Low-Voltage Programming, and why does keeping it ON disable pin RB5?
**Answer:**  
- **What it means:** Disables the **Low-Voltage In-Circuit Serial Programming (LVP)** mode.
- **Why we wrote it:** When LVP is enabled, dedicated pin `RB5/PGM` cannot be used as general-purpose digital I/O because applying a logic high voltage to `RB5` enters hardware programming mode.
- **Why turn it OFF:** Setting `LVP = OFF` reclaims pin `RB5` so it functions as a regular digital I/O pin (driving segment `f` of the Units display).

#### Q0.5: What does `void delay_ms(unsigned int ms)` mean? Why is the return type `void`? Why is `ms` declared as `unsigned int` instead of `signed int`?
**Answer:**  
- **Return Type `void`:** Indicates that this function executes an operational timing delay and does not return any computed value to the caller.
- **Parameter `unsigned int ms`:** An unsigned 16-bit integer holding values from $0$ to $65,535$. Time intervals can never be negative; using `unsigned` prevents negative value errors and doubles the maximum delay capability from $32,767\text{ ms}$ ($\sim 32\text{ s}$) to $65,535\text{ ms}$ ($\sim 65.5\text{ s}$).

#### Q0.6: In `for(j = 0; j < 165; j++);`, why is there an empty semicolon `;` at the end? Where did the number 165 come from?
**Answer:**  
- **The Empty Semicolon `;`:** Forms a null statement. The CPU simply executes the loop counter increment and condition check without executing any loop body, effectively burning CPU clock cycles to create time delay.
- **Origin of 165:** At $F_{osc} = 8\text{ MHz}$, the instruction clock is $T_{cy} = \frac{4}{F_{osc}} = 0.5\,\mu\text{s}$. The compiled assembly for one iteration of this C loop takes roughly 12 instruction cycles ($12 \times 0.5\,\mu\text{s} = 6.0\,\mu\text{s}$). To produce $1\text{ ms} = 1000\,\mu\text{s}$:
  $$\text{Iterations} \approx \frac{1000\,\mu\text{s}}{6.0\,\mu\text{s}} \approx 165\text{ iterations}$$

#### Q0.7: Why is `unsigned char seg_code[]` declared as `unsigned char` rather than `int`? What does `0x` mean in `0x3F`?
**Answer:**  
- **`unsigned char`:** An 8-bit data type ($1\text{ byte}$, range $0$ to $255$). Since microcontroller I/O ports (`PORTB`, `PORTD`) are exactly 8 bits wide, an 8-bit `unsigned char` maps directly $1:1$ to the physical hardware port register with zero wasted RAM. Using `int` ($16\text{ bits}$ in C18) would needlessly waste RAM and require extra truncation instructions.
- **`0x` Prefix:** Designates that the constant is written in **Hexadecimal (base 16)** format. `0x3F` equals binary `0011 1111` ($63$ in decimal).

#### Q0.8: Why did you declare `int tens, units;` at the very beginning of `main()`? Why couldn't you write `for(int tens = 0; ...)`?
**Answer:**  
The Microchip MPLAB C18 compiler complies with the **ANSI C89 (ISO C90)** standard. Under C89, all variable declarations within a block must appear before any executable statements. In-loop declarations like `for(int tens = 0; ...)` were only introduced in C99, and attempting to use them in C18 causes a syntax compilation error.

#### Q0.9: What do `TRISB = 0x00;` and `TRISD = 0x00;` mean? What does TRIS stand for? Why is `0` output and `1` input?
**Answer:**  
- **Meaning:** Sets all 8 pins of Port B and all 8 pins of Port D as digital outputs.
- **What TRIS stands for:** **Tri-State**. It controls the high-impedance (third state) buffers of the physical pins.
- **Why `0` is Output and `1` is Input:** Standard Microchip convention:
  - `0` looks like **O** for **Output**.
  - `1` looks like **I** for **Input**.

#### Q0.10: Why did you write `PORTB = 0x00;` and `PORTD = 0x00;` right before the loop?
**Answer:**  
Upon power-on reset, register contents can hold indeterminate states or bootloader artifacts. Writing `0x00` initializes all display pins to logic LOW ($0\text{V}$), ensuring that all segments of both Common Cathode displays start in a completely blank/OFF state before counting commences.

#### Q0.11: What does `while(1)` mean? Why `1`? Why do embedded microcontroller programs run in an infinite loop?
**Answer:**  
- **Meaning:** Creates an endless/infinite loop.
- **Why `1`:** In C, any non-zero integer evaluates to Boolean `TRUE`. Therefore, `while(1)` will never evaluate to false and will never terminate.
- **Why infinite loops are required in embedded systems:** Microcontrollers do not have an underlying desktop operating system (like Windows or Linux) to "exit" to. If `main()` reaches the end and exits, the CPU instruction pointer will increment into unprogrammed flash memory, executing random opcodes (or `0xFFFF`) and crashing or executing continuous resets. Embedded firmware must run continuously from power-on until power-down.

#### Q0.12: In `PORTD = seg_code[tens];` and `PORTB = seg_code[units];`, how does array indexing work? What physical signals are sent when `tens = 0`?
**Answer:**  
- **Array Indexing:** `seg_code[tens]` uses the current integer value of `tens` as an offset into the array `seg_code`. When `tens = 0`, it fetches index 0, which contains `0x3F`.
- **Physical Output:** `PORTD = 0x3F` writes binary `0011 1111` to Port D. This drives pins `RD0` through `RD5` (segments a, b, c, d, e, f) to $+5\text{V}$ (logic HIGH), illuminating the numeral '0', while keeping `RD6` (segment g) and `RD7` (dp) at $0\text{V}$ (OFF).

#### Q0.13: Why did you write `delay_ms(500);`? What happens if you change it to `delay_ms(5);` or omit it completely?
**Answer:**  
- **Why 500 ms:** It holds each two-digit number static for half a second, matching the human cognitive and visual perception rate.
- **If set to 5 ms or omitted:** The MCU increments the count every few microseconds. To human eyes, due to retinal Persistence of Vision (POV), all numerals from 0 to 9 will blur together into a solid glowing "88" at all times, making individual counts completely imperceptible.

---

### Level 1: Fundamental / Conceptual Questions ("What & Why")

#### Q1.1: What is a 7-segment display, and why is it preferred over LCDs in basic counters?
**Answer:**  
A 7-segment display is an optoelectronic display device composed of 8 individual light-emitting diodes (7 segments for numerals plus 1 decimal point). It is preferred in industrial meters, elevators, and numerical counters due to:
1. High luminance and readability from wide viewing angles and harsh lighting.
2. Extreme electrical and physical ruggedness compared to fragile glass liquid crystal panels.
3. Simple parallel drive electronics requiring zero complex communication initialization protocols.

#### Q1.2: Why do we have Common Cathode (CC) and Common Anode (CA) displays? What is the electrical difference?
**Answer:**  
The distinction lies in which terminal of the internal LEDs is shared:
- In **Common Cathode**, all LED cathodes are tied to a single common pin connected to Ground ($0\text{V}$). Logic `HIGH` ($+5\text{V}$) turns a segment ON (current sourcing mode).
- In **Common Anode**, all anodes are tied to $+5\text{V}$. Logic `LOW` ($0\text{V}$) turns a segment ON (current sinking mode).  
The choice depends on the microcontroller's drive capabilities: older microcontrollers (like standard 8051) have strong sinking but weak sourcing capability, favouring Common Anode, whereas modern PIC microcontrollers can source and sink up to $25\text{ mA}$ per pin, supporting both configurations.

#### Q1.3: Why are $330\,\Omega$ series resistors placed between the PIC pins and the display segments? What happens if you omit them?
**Answer:**  
LEDs have an exponential current-voltage characteristic with negligible dynamic resistance once the forward threshold voltage ($V_F \approx 1.8\text{V}$) is crossed. Without current-limiting resistors, connecting a $5\text{V}$ logic pin directly across the diode causes an excessive surge current ($I > 100\text{ mA}$), exceeding the PIC18F maximum pin rating of $25\text{ mA}$. This will overheat and permanently destroy both the microcontroller output drivers and the LED segments.

#### Q1.4: Why does the code use a 500 ms delay between increments?
**Answer:**  
The human eye cannot resolve optical state changes that happen faster than roughly 50 to 100 ms due to the biological response time of retinal photoreceptors. Without a delay, the MCU running at 8 MHz would execute millions of cycles per second, cycling from 00 to 99 in just a few microseconds, resulting in an unreadable blur of all segments lit simultaneously.

---

### Level 2: Easy / Direct Implementation Questions

#### Q2.1: How is the hexadecimal value `0x5B` derived to display the numeral '2'?
**Answer:**  
To display '2', segments **a, b, d, e, g** must be turned ON, while **c, f, dp** must remain OFF:
- `a = 1`, `b = 1`, `c = 0`, `d = 1`, `e = 1`, `f = 0`, `g = 1`, `dp = 0`
- Bit layout: `[dp, g, f, e, d, c, b, a]` = `0 1 0 1 1 0 1 1` in binary
- Binary `0101` = Hex `5`, Binary `1011` = Hex `B` $\implies$ **`0x5B`**.

#### Q2.2: What do the statements `TRISB = 0x00;` and `TRISD = 0x00;` achieve?
**Answer:**  
In PIC18F microcontrollers, the **TRIS** (Tri-State) register sets the data direction of each I/O pin:
- Writing `0` configures the pin as an **Output** driver.
- Writing `1` configures the pin as an **Input** buffer.  
`TRISB = 0x00` and `TRISD = 0x00` configure all 8 pins of Port B and all 8 pins of Port D as low-impedance digital push-pull outputs.

#### Q2.3: Why must variables `tens` and `units` be declared at the beginning of `main()` in C18?
**Answer:**  
The Microchip MPLAB C18 compiler strictly conforms to the **ANSI C89 (ISO C90)** standard. Under C89, all local variables within a block must be declared before any executable statements. Declaring variables mid-function (as permitted in C99 or C++) triggers a compilation syntax error in C18.

#### Q2.4: How does the nested `for` loop logic operate?
**Answer:**  
The outer loop indexes `tens` from `0` to `9`. For every single increment of `tens`, the inner loop iterates `units` completely from `0` through `9`. Thus:
- At `tens = 0`: `units` cycles $0, 1, 2, ... 9$ $\implies$ count displays `00` to `09`.
- At `tens = 1`: `units` cycles $0, 1, 2, ... 9$ $\implies$ count displays `10` to `19`.
- This continues until `99`, after which both loops terminate, and the enclosing `while(1)` restarts the entire sequence at `00`.

---

### Level 3: Moderate / Practical Circuit Design Questions

#### Q3.1: The current circuit uses 16 MCU pins (8 for Tens, 8 for Units). How can we modify the circuit to drive both digits using only 10 pins?
**Answer:**  
By implementing **Time-Division Multiplexing (TDM)**:
1. Connect all segment lines (a through g) of both displays in parallel to a single 8-bit port (e.g., `PORTD`).
2. Connect the Common Cathode pin of the Tens display to a switching transistor (or GPIO, e.g., `RB0`) and the Units Common Cathode to another transistor (e.g., `RB1`).
3. Alternately enable one digit at a time:
   - Output Tens pattern on `PORTD`, activate `RB0` (Tens ON, Units OFF) for $5\text{ ms}$.
   - Output Units pattern on `PORTD`, activate `RB1` (Units ON, Tens OFF) for $5\text{ ms}$.
4. At a refresh rate of $100\text{ Hz}$ ($10\text{ ms}$ full cycle), the human eye perceives both digits as continuously illuminated due to Persistence of Vision (POV).

#### Q3.2: What is Persistence of Vision (POV), and what is the critical flicker frequency?
**Answer:**  
Persistence of Vision is the optical phenomenon where the human eye retains an image on the retina for roughly $\frac{1}{20}\text{th}$ to $\frac{1}{16}\text{th}$ of a second ($50$ to $60\text{ ms}$) after the light stimulus ceases. In multiplexed displays, if each digit is refreshed at $\ge 50\text{ Hz}$ (ideally $100\text{ Hz}$ or higher), retinal persistence blends the rapid sequential flashes into a continuous, stable display with zero perceptible flicker.

#### Q3.3: Why should `LATB` and `LATD` be used instead of `PORTB` and `PORTD` when rapidly updating pins?
**Answer:**  
In PIC microcontrollers, writing to `PORTx` performs a **Read-Modify-Write (RMW)** sequence on the physical pin voltages. If pins have external capacitive loading or slow voltage rise times, reading `PORTx` may sample an adjacent pin as logic `0` before its voltage has fully reached the $V_{IH}$ threshold, accidentally clearing that bit when writing back. Writing to the **Output Latch (`LATx`)** writes directly to internal flip-flops, completely eliminating RMW hazards.

#### Q3.4: How would you add a hardware Pushbutton to pause and resume the counting?
**Answer:**  
1. Connect an active-low tactile pushbutton with a $10\text{ k}\Omega$ pull-up resistor to pin `RB0` (or `INT0`).
2. In software, poll the button state inside the counting loop:
   ```c
   if(PORTBbits.RB0 == 0) { // Button pressed
       delay_ms(20);        // Debounce delay
       while(PORTBbits.RB0 == 0); // Wait for release
       is_paused = !is_paused;   // Toggle pause state flag
   }
   while(is_paused) { /* hold display */ }
   ```
3. Alternatively, configure `INT0` external interrupt to toggle the pause flag asynchronously.

---

### Level 4: Tough / In-Depth & Troubleshooting Questions

#### Q4.1: Explain the Read-Modify-Write (RMW) hazard in detail. How does it cause intermittent display corruptions?
**Answer:**  
When an instruction like `PORTBbits.RB0 = 1;` executes in assembly:
1. The CPU executes `MOVF PORTB, W` (reads the physical voltage levels of all 8 pins).
2. Modifies bit 0 in the Working Register (`BSF WREG, 0`).
3. Writes the modified byte back (`MOVWF PORTB`).  
If segment pin `RB1` is connected to a long wire with capacitive load, its voltage may rise slowly. If `RB0` is toggled immediately after `RB1`, the CPU reads `RB1` as `0` during the read phase of the `RB0` instruction and writes back `0`, unintentionally turning off segment `RB1`. Using `LATBbits.LATB0 = 1;` avoids reading the physical pins entirely.

#### Q4.2: How would you eliminate the blocking `delay_ms(500)` using hardware Timer 0 interrupts?
**Answer:**  
Blocking software loops consume 100% of the CPU's processing bandwidth, preventing the MCU from reading buttons, sensors, or serial communication.  
To implement a non-blocking architecture:
1. Configure **Timer 0** in 16-bit mode with a 1:16 prescaler to overflow every $50\text{ ms}$.
2. In the Timer 0 Interrupt Service Routine (`ISR`), increment a software tick counter:
   ```c
   void high_priority_isr(void) {
       if(INTCONbits.TMR0IF) {
           INTCONbits.TMR0IF = 0; // Clear flag
           TMR0H = RELOAD_H; TMR0L = RELOAD_L;
           tick_50ms++;
           if(tick_50ms >= 10) { // 10 * 50ms = 500ms
               tick_50ms = 0;
               advance_count_flag = 1;
           }
       }
   }
   ```
3. The `main()` loop remains completely free to handle other tasks and only advances the digits when `advance_count_flag == 1`.

#### Q4.3: On physical hardware, the display shows `88` permanently or distorted ghosting numerals. What are the top 3 troubleshooting steps?
**Answer:**  
1. **Floating / Open Common Cathode:** If the common cathode pin is disconnected or floating, current flows between segment lines through internal ESD protection diodes, causing random segments to illuminate dimly. Verify that CC is firmly grounded.
2. **Missing or Inverted Pin Mapping:** If segments are wired in reverse order (e.g., MSB-to-LSB swapped), numeric patterns will display gibberish glyphs. Cross-check each physical MCU pin with the segment truth table using a digital multimeter in diode test mode.
3. **Ghosting due to slow switching:** In multiplexed designs, failing to blank the display (`PORTD = 0x00`) before switching digit select transistors leaves residual charge on the LED capacitances, causing the previous digit's shape to faintly appear as a ghost image on the next digit.
