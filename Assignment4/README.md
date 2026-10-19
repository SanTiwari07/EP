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
- **`1A` (Pin 2):** Direction input 1 (connected to PIC18F `RD5`).
- **`2A` (Pin 7):** Direction input 2 (connected to PIC18F `RD6`).
- **`1Y` (Pin 3) & `2Y` (Pin 6):** Motor driver outputs connected across the DC Motor terminals.
- **`VCC1` (Pin 16):** Logic supply (+5V).
- **`VCC2` (Pin 8):** Motor supply (+5V / +12V).
- **`GND` (Pins 4, 5, 12, 13):** Heat sink ground.

### 4.3 PWM Special Function Registers (SFRs)
| SFR | Description | Value in Code |
|---|---|---|
| **PR2** | Timer 2 Period Register | `0xBA` (186) |
| **TMR2** | Timer 2 Count Register | `0x00` |
| **CCP1CON** | Standard CCP1 Control Register | `0x0C` (PWM mode) |
| **T2CON** | Timer 2 Control Register | `0x07` (Timer 2 ON, Prescaler 1:16) |
| **CCPR1L** | CCP1 Register Low Byte (Duty Cycle MSBs) | `0x25`, `0x4B`, `0x25`, `0x94` |

---

## 5. Mathematical Calculations for PWM Operation

### 5.1 Given System Parameters
- Crystal Frequency ($F_{osc}$): $48\text{ MHz}$
- Target PWM Frequency ($F_{pwm}$): $4\text{ kHz}$
- Timer 2 Prescaler: $16$

### 5.2 Period Register PR2 Calculation
$$\text{PR2} = \left[ \frac{48,000,000}{4 \times 4000 \times 16} \right] - 1 = 187.5 - 1 = 186.5 \approx 186 = \mathbf{\text{0xBA}}$$

---

## 6. Duty Cycle Steps in Code (`exp4.c`)

| Step | CCPR1L (Hex) | CCP1CONbits.DC1B1 | CCP1CONbits.DC1B0 | Delay |
|:---:|:---:|:---:|:---:|:---:|
| **1** | `0x25` | `1` | `0` | `MSdelay(2000)` |
| **2** | `0x4B` | `0` | `0` | `MSdelay(2000)` |
| **3** | `0x25` | `1` | `0` | `MSdelay(2000)` |
| **4** | `0x94` | `1` | `0` | `MSdelay(2000)` |

---

## 7. Interfacing Connections
- `TRISCbits.TRISC2 = 0` (`RC2/CCP1` as PWM output)
- `TRISD = 0` (`PORTD` as output)
- `PORTDbits.RD5 = 1`, `PORTDbits.RD6 = 0` (Motor direction control)

---

## 8. Build and Verification Instructions

### Using MPLAB C18 Toolchain
```powershell
# Compile C source
mcc18.exe -p=18F4550 "exp4.c" -fo="exp4.o"

# Link using relocated linker script
mplink.exe /p18F4550 "..\rm18f4550.lkr" "exp4.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_4.cof" /M"exp_4.map" /W
```
