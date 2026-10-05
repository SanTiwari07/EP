# Experiment 4: Generation of PWM Signal (Motor Speed Control)

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Problem Statement
Design and develop a PWM-based motor speed control system using CCP PWM mode using PIC18F4550 microcontroller.

---

## 2. Objectives
a. To understand the working of PWM.  
b. To study the on-chip CCP Module (PWM section) of PIC Microcontroller.  
c. To interface DC motor, via Driver IC L293D, to PIC Microcontroller.  
d. Apply the PWM signal to control the speed of DC Motor.  

---

## 3. S/W Packages and H/W Used
- **Software:** MPLAB IDE v8.92 / MPLAB X, MPLAB C18 Compiler v3.47, Proteus VSM
- **Hardware:** Universal Board of PIC18F4550, L293D Motor Driver IC, DC Motor, Power Supply

---

## 4. Theory & Peripheral Description

### 4.1 CCP Module (Capture / Compare / PWM)
The PIC18F4550 features on-chip CCP modules (`CCP1` on pin `RC2` and `CCP2` on pin `RC1` or `RB3`). Each CCP module is controlled via its control register (`CCPxCON`) and data registers (`CCPRxL` and `CCPRxH`).

In **PWM Mode**, the module produces up to 10-bit resolution PWM outputs using **Timer 2** as its dedicated time base:
- **PWM Period ($T_{pwm}$):** Governed by the `PR2` register and Timer 2 prescaler.
- **PWM Duty Cycle ($T_{on}$):** Governed by 10 bits: the upper 8 bits in `CCPR1L` and the lower 2 bits in `CCP1CON<5:4>` (`DC1B1:DC1B0`).

### 4.2 L293D Motor Driver
The L293D is a quadruple high-current half-H driver designed to supply bidirectional drive currents of up to 600 mA at voltages from 4.5 V to 36 V to inductive loads:
- **`1,2EN` (Pin 1):** Enable input for Drivers 1 and 2 (connected to PIC18F PWM pin `RC2/CCP1`).
- **`1A` (Pin 2):** Direction input 1 (connected to PIC18F `RB0`).
- **`2A` (Pin 7):** Direction input 2 (connected to PIC18F `RB1`).
- **`1Y` (Pin 3) & `2Y` (Pin 6):** Motor driver outputs connected across the DC Motor terminals.
- **`VCC1` (Pin 16):** Logic supply (+5V).
- **`VCC2` (Pin 8):** Motor supply (+5V / +12V).
- **`GND` (Pins 4, 5, 12, 13):** Heat sink ground.

### 4.3 PWM Special Function Registers (SFRs)
| SFR | Description | Reset Value | Address |
|---|---|---|---|
| **CCP1CON** | Standard CCP1 Control Register | `0x00` | `0xFBD` |
| **CCPR1L** | CCP1 Register Low Byte (Duty Cycle MSBs) | `0x00` | `0xFBE` |
| **CCPR1H** | CCP1 Register High Byte (PWM buffer) | Unknown | `0xFBF` |
| **T2CON** | Timer 2 Control Register | `0x00` | `0xFCA` |
| **TMR2** | Timer 2 Count Register | `0x00` | `0xFCC` |
| **PR2** | Timer 2 Period Register | `0xFF` | `0xFCB` |

#### CCP1CON Register Format
| Bit | Name | Function in PWM Mode | Value |
|---|---|---|---|
| D7-D6 | — | Unimplemented (Read as `0`) | `00` |
| D5-D4 | **DC1B1:DC1B0** | 2 LSBs of 10-bit PWM Duty Cycle | Dynamically loaded |
| D3-D0 | **CCP1M3:CCP1M0** | CCP Mode Select bits (`11xx` = PWM mode) | `1100` (`0x0C`) |

#### T2CON Register Format
- **Bit 2 (`TMR2ON`):** Timer 2 ON (`1` = ON).
- **Bits 1-0 (`T2CKPS1:T2CKPS0`):** Timer 2 Prescaler (`10` = 1:16 prescaler).
- **Bits 6-3 (`T2OUTPS3:T2OUTPS0`):** Postscaler (`0000` = 1:1 postscaler).
$$\text{T2CON} = \text{0b00000110} = \text{0x06}$$

---

## 5. Mathematical Calculations for PWM Operation (Section 5, Page 4.11)

### 5.1 Given System Parameters
- Crystal Frequency ($F_{osc}$): $48\text{ MHz}$
- Oscillator Period ($T_{osc}$): $\frac{1}{48\text{ MHz}} = 0.020833\ \mu\text{s}$
- Target PWM Frequency ($F_{pwm}$): $4\text{ kHz} = 4,000\text{ Hz}$
- Target PWM Period ($T_{pwm}$): $\frac{1}{4\text{ kHz}} = 250\ \mu\text{s}$
- Timer 2 Prescaler: $16$

### 5.2 Period Register PR2 Calculation
$$T_{pwm} = (\text{PR2} + 1) \times 4 \times T_{osc} \times (\text{TMR2 Prescaler})$$

Rearranging for $\text{PR2}$:
$$\text{PR2} = \left[ \frac{F_{osc}}{4 \times F_{pwm} \times (\text{TMR2 Prescaler})} \right] - 1$$

$$\text{PR2} = \left[ \frac{48,000,000}{4 \times 4000 \times 16} \right] - 1 = \left[ \frac{48,000,000}{256,000} \right] - 1 = 187.5 - 1 = \mathbf{186.5}$$

Rounding to nearest integer:
- Selecting **$\mathbf{PR2 = 186}$**:
  $$T_{pwm} = (186 + 1) \times 4 \times \left(\frac{1}{48\text{ MHz}}\right) \times 16 = 187 \times \frac{64}{48\text{ MHz}} = 249.33\ \mu\text{s} \implies F_{pwm} \approx 4.01\text{ kHz}$$
- Selecting **$\mathbf{PR2 = 187}$**:
  $$T_{pwm} = (187 + 1) \times 4 \times \left(\frac{1}{48\text{ MHz}}\right) \times 16 = 188 \times \frac{64}{48\text{ MHz}} = 250.67\ \mu\text{s} \implies F_{pwm} \approx 3.989\text{ kHz}$$

### 5.3 10-Bit PWM Duty Cycle Calculation
$$\text{CCPR1L}:\text{CCP1CON}\langle 5:4 \rangle = \left[ \frac{\%DC_{pwm} \times F_{osc}}{100 \times F_{pwm} \times (\text{TMR2 Prescaler})} \right]$$

$$\text{Multiplier} = \frac{48,000,000}{100 \times 4000 \times 16} = \frac{48,000,000}{6,400,000} = \mathbf{7.5}$$

$$\mathbf{\text{10-Bit Duty Cycle Value} = \%DC_{pwm} \times 7.5}$$

---

## 6. Official Duty Cycle Calculation Table (Page 4.11)

| Duty Cycle (%$DC$) | 10-Bit Value ($\%DC \times 7.5$) | Binary (10-bit) | CCPR1L Value (Hex / Dec) | CCP1CON<5:4> (DC1B1:DC1B0) |
|:---:|:---:|:---:|:---:|:---:|
| **20%** | $20 \times 7.5 = \mathbf{150}$ | `00 1001 0110` | `0x25` (37) | `10` (`DC1B1=1, DC1B0=0`) |
| **40%** | $40 \times 7.5 = \mathbf{300}$ | `01 0010 1100` | `0x4B` (75) | `00` (`DC1B1=0, DC1B0=0`) |
| **60%** | $60 \times 7.5 = \mathbf{450}$ | `01 1100 0010` | `0x70` (112) | `10` (`DC1B1=1, DC1B0=0`) |
| **80%** | $80 \times 7.5 = \mathbf{600}$ | `10 0101 1000` | `0x96` (150) | `00` (`DC1B1=0, DC1B0=0`) |
| **100%** | $100 \times 7.5 = \mathbf{750}$ | `10 1110 1110` | `0xBB` (187) | `10` (`DC1B1=1, DC1B0=0`) |

---

## 7. Interfacing Diagram
```text
  PIC18F4550 Microcontroller                        L293D Motor Driver
+----------------------------+                     +-------------------+
|                            |                     | 1                 |
|            RC2/CCP1 (Pin 17)-------------------->| 1,2EN             |
|                            |                     |                   |
|                 RB0 (Pin 33)-------------------->| 1A (Pin 2)        |
|                            |                     |                   |
|                 RB1 (Pin 34)-------------------->| 2A (Pin 7)        |
|                            |                     |                   |
|                            |                     | 1Y (Pin 3) -------+----> [DC Motor +]
|                            |                     |                   |
|                            |                     | 2Y (Pin 6) -------+----> [DC Motor -]
|                            |                     |                   |
|                            |                     | Pins 4,5,12,13 ---+----> GND
|                            |                     | Pin 16 (VCC1) ----+----> +5V (Logic)
|                            |                     | Pin 8  (VCC2) ----+----> +12V/+5V (Motor)
+----------------------------+                     +-------------------+
```

---

## 8. Build and Verification Instructions

### Using MPLAB C18 Toolchain
```powershell
# Compile C source
mcc18.exe -p=18F4550 "exp4.c" -fo="exp4.o"

# Link using relocated linker script
mplink.exe /p18F4550 "..\rm18f4550.lkr" "exp4.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_4.cof" /M"exp_4.map" /W
```
