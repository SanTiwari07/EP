# Experiment 5: 16x2 Alphanumeric LCD Interfacing with PIC18F4550

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Problem Statement
Interface a 16x2 LCD module to PIC18F4550 microcontroller and write a program in Embedded C to display characters on the 16x2 LCD display.
---

## 2. Objectives
a. To understand the internal working and architecture of the 16x2 alphanumeric LCD module.  
b. To study the LCD control section and GPIO ports of PIC Microcontroller.  
c. To interface the LCD module to PIC Microcontroller in 8-bit mode (using parallel data lines rather than I2C).  

---

## 3. S/W Packages and H/W Used
- **Software:** MPLAB IDE v8.92 / MPLAB X, MPLAB C18 Compiler v3.47, Proteus VSM
- **Hardware:** Universal Development Board of PIC18F4550, PICkit3 Programmer / USB HID Bootloader, 16x2 Alphanumeric LCD Display (HD44780 or equivalent), 10 k$\Omega$ Potentiometer for contrast control.

---

## 4. Theory & Peripheral Description

### 4.1 16x2 Alphanumeric LCD Module Overview
The standard 16x2 LCD display can display two lines with 16 characters on each line. Each character is rendered in a $5 \times 7$ pixel matrix.

The module possesses 16 interface pins:
- **8 Data Pins ($D_0 - D_7$):** Transmit 8-bit ASCII characters or 8-bit command codes.
- **3 Control Pins ($RS, R/\overline{W}, EN$):**
  - **$RS$ (Register Select):**
    - $RS = 0$: Command Register (commands like clear display, set cursor, etc.).
    - $RS = 1$: Data Register (ASCII characters to be rendered on screen).
  - **$R/\overline{W}$ (Read/Write):**
    - $R/\overline{W} = 0$: Write operation (PIC sends data/command to LCD).
    - $R/\overline{W} = 1$: Read operation (reading status/busy flag).
  - **$EN$ (Enable):** Latches the information on the falling edge (high-to-low transition pulse).
- **5 Power & Contrast Pins:**
  - $V_{SS}$ (Pin 1): Ground ($0\text{ V}$).
  - $V_{DD}$ (Pin 2): Positive Power Supply ($+5\text{ V}$).
  - $V_{EE}$ (Pin 3): Contrast Adjustment (connected to 10 k$\Omega$ potentiometer wiper).
  - $A / \text{LED+}$ (Pin 15): Backlight Anode ($+5\text{ V}$).
  - $K / \text{LED-}$ (Pin 16): Backlight Cathode (Ground).

### 4.2 Standard Command Set (Section 1.1, Page 9.3)
| Command Code (Hex) | Function / Description |
|---|---|
| **0x38** | Function Set: 8-bit data interface, 2 display lines, $5 \times 7$ character font |
| **0x0E** | Display ON, Cursor ON |
| **0x0C** | Display ON, Cursor OFF |
| **0x01** | Clear Entire Display and reset cursor to home |
| **0x06** | Entry Mode: Increment cursor position automatically from left to right |
| **0x80** | Set DDRAM Address: Beginning of 1st row (columns $00_{16}$ to $0F_{16}$) |
| **0xC0** | Set DDRAM Address: Beginning of 2nd row (columns $40_{16}$ to $4F_{16}$) |

### 4.3 Flowchart / Steps for Command and Data Operations (Page 9.3)

#### Steps for Sending a Command:
1. Place the command byte on the data bus (`PORTD = command`).
2. Make $RS = 0$ (select Command Register).
3. Make $R/\overline{W} = 0$ (configure for Write).
4. Send an Enable pulse: set $EN = 1$, wait brief delay, then set $EN = 0$ (latches on falling edge).
5. Call delay (typically $5 - 10\text{ ms}$).

#### Steps for Sending Data (ASCII Character):
1. Place ASCII data byte on data bus (`PORTD = data`).
2. Make $RS = 1$ (select Data Register).
3. Make $R/\overline{W} = 0$ (configure for Write).
4. Send an Enable pulse: set $EN = 1$, wait brief delay, then set $EN = 0$ (latches on falling edge).
5. Call delay.

---

## 5. Circuit Interfacing Diagram (Page 9.4)

```text
                       +5V
                        |
       +----------------+----------------+
       |                |                |
     Pin 2 (VDD)      Pin 15 (A)     10k Pot
  +----+----+       +----+----+      +-------+
  |         |       |         |      |       |
  | 16x2    +-------+         +------+   [Wiper] ---> Pin 3 (VEE) Contrast
  | LCD     |                 |      |       |
  | MODULE  |                 |      +-------+
  |         +-------+         +----------+
  +----+----+       +----+----+          |
     Pin 1 (VSS)      Pin 16 (K)        GND
       |                |
      GND              GND

  PIC18F4550 Microcontroller                     16x2 LCD Display Pins
+----------------------------+                  +----------------------+
|                            |                  |                      |
|                RE0 (Pin 8) +----------------->| Pin 4  (RS)          |
|                RE1 (Pin 9) +----------------->| Pin 5  (RW)          |
|               RE2 (Pin 10) +----------------->| Pin 6  (EN)          |
|                            |                  |                      |
|               RD0 (Pin 19) +----------------->| Pin 7  (D0)          |
|               RD1 (Pin 20) +----------------->| Pin 8  (D1)          |
|               RD2 (Pin 21) +----------------->| Pin 9  (D2)          |
|               RD3 (Pin 22) +----------------->| Pin 10 (D3)          |
|               RD4 (Pin 27) +----------------->| Pin 11 (D4)          |
|               RD5 (Pin 28) +----------------->| Pin 12 (D5)          |
|               RD6 (Pin 29) +----------------->| Pin 13 (D6)          |
|               RD7 (Pin 30) +----------------->| Pin 14 (D7)          |
|                            |                  |                      |
+----------------------------+                  +----------------------+
```

---

## 6. Algorithm
1. Configure all analog pins as digital I/O by setting `ADCON1 = 0x0F`.
2. Configure PORTD as output (`TRISD = 0x00`) for the 8-bit data bus ($D_0 - D_7$).
3. Configure PORTE pins RE0, RE1, RE2 as outputs (`TRISE = 0x00`) for $RS, R/\overline{W}, EN$.
4. Initialize the LCD module:
   - Send `0x38` (2 lines, $5\times 7$ character font, 8-bit mode).
   - Send `0x01` (clear display).
   - Send `0x0E` (turn on display and cursor).
   - Send `0x06` (set entry mode to auto-increment cursor).
   - Send `0x80` (set cursor to starting position on Row 1).
5. Transmit the ASCII character string `"HELLO WORLD"` sequentially to the data register.
6. Enter an idle loop (`while(1)`) while the message is displayed stably.

---

## 7. Build Instructions

### Using MPLAB C18 Toolchain
```powershell
# Compile C source
mcc18.exe -p=18F4550 "exp5.c" -fo="exp5.o"

# Link using relocated linker script
mplink.exe /p18F4550 "..\rm18f4550.lkr" "exp5.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_5.cof" /M"exp_5.map" /W
```
