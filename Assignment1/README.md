# Experiment 1: Interfacing LEDs with 89C51 / 8051 Microcontroller

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Problem Statement
Develop and implement embedded C programs for the 8051 (AT89C51 / AT89S52) microcontroller to accomplish the following:
- **Part A:** Toggle all LEDs interfaced to Port P1 of 8051 continuously with a delay of 100 ms using software looping.
- **Part B:** Perform up/down counting of two-digit hexadecimal numbers (0x00 to 0xFF) and display the count on LEDs connected to PORT P1.
- **Part C:** Perform up/down counting of two-digit BCD numbers (00 to 99) and display the result on LEDs connected to PORT P1.
- **Part D:** Implement LED Chasing (ring shift left and right) across the 8 LEDs connected to PORT P1.
---

## 2. Objectives
a. To study the Port structure of 8051 Microcontroller.  
b. To study LED interfacing and current-sinking vs current-sourcing drive techniques.  
c. To understand the Keil $\mu$Vision IDE workflow.  
d. To study calibrated software delay generation using nested loops.  
e. To implement and analyze binary, hexadecimal, and BCD counters.  

---

## 3. S/W Packages and H/W Used
- **Software:** Keil $\mu$Vision IDE (C51 Compiler), Proteus VSM (ISIS) Simulator, Windows 10/11
- **Hardware:** AT89C51 / AT89S52 Microcontroller, 11.0592 MHz Crystal Oscillator, 33 pF Capacitors, 8 LEDs, 470 $\Omega$ Resistor Network, 10 k$\Omega$ Resistor, 10 $\mu$F Reset Capacitor.

---

## 4. Theory & Port Architecture

### 4.1 8051 I/O Port Organization
The 8051 contains four 8-bit bidirectional parallel I/O ports ($P_0, P_1, P_2, P_3$):
- **Port 0 ($P_0$):** Open-drain bi-directional I/O port. When used as GPIO, it **requires external $10\text{ k}\Omega$ pull-up resistors** because it lacks internal pull-ups.
- **Port 1 ($P_1$):** Dedicated 8-bit quasi-bidirectional I/O port with **internal pull-up resistors**. Ideal for direct interfacing with active-low LED loads.
- **Port 2 ($P_2$):** Quasi-bidirectional port with internal pull-ups, multiplexed as the higher-order address bus ($A_8 - A_{15}$).
- **Port 3 ($P_3$):** Quasi-bidirectional port with internal pull-ups, multiplexed with special peripheral functions (UART $RXD/TXD$, External Interrupts $INT0/INT1$, Timers $T0/T1$, and Bus Control $\overline{WR}/\overline{RD}$).

### 4.2 Delay Calculation (Section 1.4, Page 1.3)
- **Crystal Oscillator Frequency ($XTAL$):** $11.0592\text{ MHz}$
- The 8051 divides the crystal frequency by $12$ to yield the internal machine cycle clock:
  $$\text{Clock Frequency} = \frac{11.0592\text{ MHz}}{12} = 921.6\text{ kHz}$$
- **Machine Cycle Period ($MC$):**
  $$MC = \frac{1}{921.6\text{ kHz}} \approx 1.085\ \mu\text{s}$$
- **Calibrated Software Delay Subroutine:**
  ```c
  void delay(unsigned int time)
  {
      unsigned int i, j;
      for (i = 0; i < time; i++)
          for (j = 0; j < 1275; j++);  /* Calibrated for ~1 ms per count */
  }
  ```
  Passing `time = 100` generates a precision delay of approximately $100\text{ ms}$.

### 4.3 Number Representation
- **Hexadecimal Representation:** Two hexadecimal digits span $00_{16}$ to $FF_{16}$ ($0$ to $255_{10}$). The upper nibble ($P_{1.7} - P_{1.4}$) represents the MSB hex digit and the lower nibble ($P_{1.3} - P_{1.0}$) represents the LSB hex digit.
- **Packed BCD Representation:** Two decimal digits ($00$ to $99$) packed into one byte:
  $$\text{Port Byte} = (\text{tens} \ll 4) \mid \text{units}$$
  Values with nibbles between $A_{16}$ and $F_{16}$ are skipped to adhere strictly to decimal notation.

---

## 5. Circuit Interfacing Diagram (Page 1.5)

```text
               +5V (Vcc)
                  |
     +------------+------------+
     |   |   |   |   |   |   |
    [>| [>| [>| [>| [>| [>| [>|   8x LEDs (Anodes tied to +5V)
     |   |   |   |   |   |   |
    [R] [R] [R] [R] [R] [R] [R]   8x 470-Ohm Resistors (Current Limiting)
     |   |   |   |   |   |   |
     |   |   |   |   |   |   |
  +--+---+---+---+---+---+---+--+
  | P1.0 P1.1 P1.2 ... P1.7     |
  | (Pins 1 - 8)                |
  |                             |
  |           AT89C51           |
  |                             |
  | Pin 19 (XTAL1) ---+         |
  |                   | [X1]    |  Crystal: 11.0592 MHz
  | Pin 18 (XTAL2) ---+ 11.0592 |  Capacitors: C1, C2 = 33 pF to GND
  |                             |
  | Pin 9  (RST)    <--- [Reset Network: 10 uF to +5V, 10k to GND]
  | Pin 31 (EA/VPP) <--- Tied to +5V (Execute from internal ROM)
  | Pin 20 (GND)    <--- Ground
  | Pin 40 (VCC)    <--- +5V Supply
  +-----------------------------+
```

---

## 6. Algorithms (Section 2, Page 1.4)

### 2.1 Part A: LED Toggling
1. Start.
2. Send low logic data on port pins (`P1 = 0x00`).
3. Call delay subroutine for 100 ms.
4. Send high logic data on port pins (`P1 = 0xFF`).
5. Call delay subroutine for 100 ms.
6. Repeat steps 2–5 continuously in an infinite loop.

### 2.2 Part B: Hexadecimal Up/Down Counter
1. Start.
2. Initialize 8-bit count to `0x00`.
3. Output count value to PORT 1 (`P1 = count`).
4. Call delay subroutine for 100 ms.
5. Increment count until reaching `0xFF`.
6. Once `0xFF` is reached, decrement count until reaching `0x00`.
7. Repeat continuously in an infinite loop.

### 2.3 Part C: BCD Up/Down Counter
1. Start.
2. Initialize `tens = 0` and `units = 0`.
3. Pack BCD number: `P1 = (tens << 4) | units`.
4. Call delay subroutine for 100 ms.
5. Increment `units` from 0 to 9. When `units` rolls over, increment `tens` from 0 to 9 (Count 00 to 99).
6. Perform decrement sequence from 99 down to 00.
7. Repeat continuously in an infinite loop.

### 2.4 Part D: LED Chasing
1. Start.
2. Initialize `P1 = 0x01` (start at P1.0).
3. Call delay subroutine for 100 ms.
4. Shift port bits left by 1 position (`P1 = P1 << 1`) in a loop of 8 iterations.
5. Initialize `P1 = 0x80` (start at P1.7).
6. Shift port bits right by 1 position (`P1 = P1 >> 1`) in a loop of 8 iterations.
7. Repeat continuously in an infinite loop.
