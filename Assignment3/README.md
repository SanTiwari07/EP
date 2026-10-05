# Experiment 3: Generation of Square Wave Using Timer 0 Interrupt

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Problem Statement
Implement Timer 0 interrupt concept using PIC18F4550 microcontroller to generate a square wave of **10 Hz** frequency.

> [!TIP]
> View interactive vector schematic and run the live web simulator in **[`interfacing_diagrams.html`](../interfacing_diagrams.html)**.

---

## 2. Objectives
a. To understand the basic concepts of Timer and Counter.  
b. To study in detail Timer 0 of PIC Microcontroller.  
c. To study the interrupt structure of PIC Microcontroller.  
d. To use timer interrupt and its related Special Function Registers (SFRs).  

---

## 3. S/W Packages and H/W Used
- **Software:** MPLAB IDE v8.92 / MPLAB X, MPLAB C18 Compiler v3.47, Proteus VSM
- **Hardware:** Universal Board of PIC18F4550, Digital Storage Oscilloscope (DSO), PICkit3 programmer / USB HID Bootloader

---

## 4. Theory & Peripheral Description

### 4.1 Timer 0 Module Features
- Software selectable operation as a timer or counter in both 8-bit or 16-bit modes.
- Readable and writable registers (`TMR0L` and `TMR0H`).
- Dedicated 8-bit, software-programmable prescaler (1:2 to 1:256).
- Selectable clock source: internal instruction cycle clock ($F_{osc}/4$) or external clock via pin `T0CKI` (`RA4`).
- Edge select for external clock source.
- Dedicated interrupt generation on overflow (`TMR0IF` in `INTCON`).

### 4.2 Special Function Registers (SFRs)
| SFR | Description | Reset Value | Address |
|---|---|---|---|
| **T0CON** | Timer0 Control Register | `0xFF` | `0xFD5` |
| **TMR0L** | Timer0 Register Low Byte | Unknown | `0xFD6` |
| **TMR0H** | Timer0 Register High Byte Buffer | `0x00` | `0xFD7` |
| **INTCON** | Interrupt Control Register | `0x00` | `0xFF2` |
| **INTCON2** | Interrupt Control Register 2 | `0xFF` | `0xFF1` |

#### T0CON: Timer0 Control Register Bit Layout
| Bit | Name | Description | Value in Exp 3 |
|---|---|---|---|
| D7 | **TMR0ON** | Timer0 On/Off Control Bit (1 = On, 0 = Off) | `1` (started in code) |
| D6 | **T08BIT** | Timer0 8-Bit/16-Bit Control Bit (0 = 16-bit, 1 = 8-bit) | `0` (16-bit mode) |
| D5 | **T0CS** | Timer0 Clock Source Select Bit (0 = Internal $F_{osc}/4$, 1 = Transition on T0CKI) | `0` (Internal clock) |
| D4 | **T0SE** | Timer0 Source Edge Select Bit (0 = Low-to-high, 1 = High-to-low) | `0` |
| D3 | **PSA** | Timer0 Prescaler Assignment Bit (0 = Prescaler assigned, 1 = Not assigned) | `0` (Assigned) |
| D2-D0 | **TOPS2:TOPS0** | Timer0 Prescaler Select: `011` = 1:16 prescale | `011` (1:16) |

$$\text{Initial T0CON} = \text{0b00000011} = \text{0x03} \quad (\text{0x83 when TMR0ON is set})$$

#### INTCON: Interrupt Control Register Bit Layout
- **GIE/GIEH (Bit 7):** Global Interrupt Enable (`1` = Enabled).
- **TMR0IE (Bit 5):** Timer0 Overflow Interrupt Enable (`1` = Enabled).
- **TMR0IF (Bit 2):** Timer0 Overflow Interrupt Flag (`1` = Overflow occurred, cleared in ISR to `0`).

---

## 5. Mathematical Calculations for 10 Hz Square Wave

### 5.1 System Specifications
- **Target Wave Frequency ($F$):** $10\text{ Hz}$
- **Total Time Period ($T$):** 
  $$T = \frac{1}{F} = \frac{1}{10\text{ Hz}} = 0.1\text{ s} = 100\text{ ms} = 100,000\ \mu\text{s}$$
- **Square Wave Duty Cycle:** $50\%$
- **Half-Cycle Delay (Time per toggle, $T_d$):**
  $$T_d = \frac{T}{2} = \frac{100\text{ ms}}{2} = 50\text{ ms} = 50,000\ \mu\text{s}$$
- **Microcontroller Crystal Frequency ($F_{osc}$):** $48\text{ MHz}$
- **Internal Instruction Frequency ($F_{cy}$):** 
  $$F_{cy} = \frac{F_{osc}}{4} = \frac{48\text{ MHz}}{4} = 12\text{ MHz}$$
- **Instruction Clock Period ($T_p$):** 
  $$T_p = \frac{1}{F_{cy}} = \frac{4}{F_{osc}} = \frac{1}{12\text{ MHz}} = \frac{1}{12}\ \mu\text{s} \approx 0.08333\ \mu\text{s}$$

### 5.2 Prescaler and Count Derivation
Using **Prescaler = 1:16**:
- **Equivalent Timer Period ($T_{eq}$):**
  $$T_{eq} = T_p \times \text{Prescaler} = \left(\frac{1}{12}\ \mu\text{s}\right) \times 16 = \frac{16}{12}\ \mu\text{s} = \frac{4}{3}\ \mu\text{s} \approx 1.3333\ \mu\text{s}$$
- **Number of Counts Needed ($n$):**
  $$n = \frac{T_d}{T_{eq}} = \frac{50,000\ \mu\text{s}}{\frac{4}{3}\ \mu\text{s}} = \frac{50,000 \times 3}{4} = 37,500\text{ counts}$$
- **Initial 16-Bit Register Count:**
  $$\text{Count} = 65,536 - n = 65,536 - 37,500 = 28,036$$
- **Hexadecimal Conversion:**
  $$28,036 \div 256 = 109\ (\text{Remainder } 132)$$
  $$109_{10} = \text{0x6D} \implies \mathbf{TMR0H = \text{0x6D}}$$
  $$132_{10} = \text{0x84} \implies \mathbf{TMR0L = \text{0x84}}$$
  $$\mathbf{\text{Initial Value} = \text{0x6D84}}$$

---

## 6. Algorithm
1. Configure Port pin `RB0` as digital output and initialize its output value to `0`.
2. Load value `0x03` into `T0CON` to configure Timer 0 in 16-bit mode, internal instruction clock ($F_{osc}/4$), and 1:16 prescaler.
3. Load registers `TMR0H` first (`0x6D`) and then `TMR0L` (`0x84`) with calculated initial count values.
4. Enable the Timer 0 Interrupt (`INTCONbits.TMR0IE = 1`) and Global Interrupt (`INTCONbits.GIE = 1`). Clear initial interrupt flag (`INTCONbits.TMR0IF = 0`).
5. Start the timer by setting `T0CONbits.TMR0ON = 1`.
6. Write the Interrupt Service Routine (ISR) at relocated interrupt vector `0x1008` containing:
   - Check if `TMR0IF == 1`.
   - Toggle the PORT pin `RB0` (`PORTBbits.RB0 = ~PORTBbits.RB0`).
   - Clear the `TMR0IF` flag for the next cycle.
   - Reload `TMR0H` (`0x6D`) first, then `TMR0L` (`0x84`).
7. Observe and verify the 10 Hz square wave (100 ms period, 50 ms ON / 50 ms OFF) on the DSO connected to pin `RB0`.

---

## 7. Build and Verification Instructions

### Using MPLAB C18 Toolchain
```powershell
# Compile C source
mcc18.exe -p=18F4550 "exp3.c" -fo="exp3.o"

# Link using relocated linker script
mplink.exe /p18F4550 "..\rm18f4550.lkr" "exp3.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_3.cof" /M"exp_3.map" /W
```
