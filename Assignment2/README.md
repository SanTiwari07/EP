# Experiment 2: Interfacing Pushbuttons, LEDs, Relay & Buzzer with PIC18F4550

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Problem Statement
Interface Pushbuttons, LEDs, Relay, and Buzzer to PIC18F4550 microcontroller. Write a program in Embedded C to interact with peripherals as follows:
- **Part a:** LEDs start chasing from left to right (RD0 to RD7) and turn **ON** the Relay and Buzzer whenever pushbutton 1 (**Switch SW0 / RB1**) is pressed.
- **Part b:** LEDs start chasing from right to left (RD7 to RD0) and turn **OFF** the Relay and Buzzer whenever pushbutton 2 (**Switch SW1 / RB0**) is pressed.
---

## 2. Objectives
a. To understand the PORT Structure of PIC Microcontroller.  
b. To study the Special Function Registers (SFRs) used to control PORT pins: `TRIS`, `PORT`, and `LAT`.  
c. To interface common peripherals like pushbuttons, LEDs, electromagnetic relay, and piezoelectric buzzer.  
d. To understand the use of MPLAB IDE and MPLAB C18 Compiler.  
e. To write and debug structured programs in Embedded C.  

---

## 3. S/W Packages and H/W Used
- **Software:** MPLAB IDE v8.92 / MPLAB X, MPLAB C18 Compiler v3.47, Proteus VSM
- **Hardware:** Explore PIC Development Board / Universal PIC18F4550 Trainer, 12V/5V SPDT Relay, 5V Piezo Buzzer, 8x LEDs, Pushbuttons (Tactile Switches), NPN Driver Transistors (BC547), 1N4007 Flyback Diodes, Current Limiting Resistors (470 $\Omega$, 1 k$\Omega$, 10 k$\Omega$).

---

## 4. Theory & Peripheral Description

### 4.1 PIC18F4550 I/O Port Registers
Each parallel I/O port on the PIC18F microcontroller is associated with three primary registers:
1. **TRIS Register (Data Direction):** Sets pin direction. Writing `1` configures the pin as an **Input** (high-impedance); writing `0` configures the pin as an **Output** driver.
2. **PORT Register (Data Read):** Reads physical voltage levels present on the device pins.
3. **LAT Register (Output Data Latch):** Holds the output state driven onto the pins. Writing to `LAT` avoids read-modify-write hazards inherent in direct `PORT` bit manipulation.

### 4.2 Port B Weak Pull-Up Resistors
Port B pins feature programmable internal weak pull-up resistors. Setting `INTCON2bits.RBPU = 0` enables pull-ups across all input pins of PORTB, ensuring stable active-low pushbutton readings without floating input states.

### 4.3 Relay Driver Circuit
Because inductive relay coils require currents higher than the microcontroller's 25 mA output capability, an NPN transistor (BC547) is used as a low-side saturated switch:
- Microcontroller pin `RC1` provides base current through a $1\text{ k}\Omega$ current-limiting resistor.
- A **flyback diode (1N4007)** is connected in reverse-parallel across the relay coil to safely suppress high-voltage back-EMF inductive spikes generated upon de-energizing.

### 4.4 Buzzer Driver Circuit
A 5V piezoelectric buzzer is driven through pin `RC2` using an NPN transistor (Q2) and a $1\text{ k}\Omega$ base resistor ($R_9$).

---

## 5. Circuit Interfacing Diagram (Page 2.8)

```text
                                        +12V / +5V (Relay Supply)
                                            |
                                            +-----+------+
                                            |     |      |
                                           [D1]  [COIL]  | (COM)
                                          1N4007  RELAY  o
                                            |     |       \   NO (Normally Open)
                                            +-----+        o--------> [Load]
                                            |             o   NC (Normally Closed)
                                           /  (Collector)
                  1k Resistor             | Q1 (NPN)
     RC1 (Pin 16) ---[ 1k ]-------------->|   (Base)
                                          \v  (Emitter)
                                            |
                                           GND
                                        +5V (Buzzer Supply)
                                            |
                                          [BUZ] Piezo Buzzer
                                            |
                                           /
                  1k Resistor (R9)        | Q2 (NPN)
     RC2 (Pin 17) ---[ 1k ]-------------->|
                                          \v
                                            |
                                           GND

  +-----------------------------+
  | PIC18F4550                  |
  |                             |
  | Pin 34 (RB1) <--- [SW0 to GND with 10k pull-up] (Pushbutton 1)
  | Pin 33 (RB0) <--- [SW1 to GND with 10k pull-up] (Pushbutton 2)
  |                             |
  | Pin 16 (RC1) ---> Relay Transistor Base
  | Pin 17 (RC2) ---> Buzzer Transistor Base
  |                             |
  | Pin 19 (RD0) ---> [470R] ---> [LED0] ---> GND
  | Pin 20 (RD1) ---> [470R] ---> [LED1] ---> GND
  | Pin 21 (RD2) ---> [470R] ---> [LED2] ---> GND
  | Pin 22 (RD3) ---> [470R] ---> [LED3] ---> GND
  | Pin 27 (RD4) ---> [470R] ---> [LED4] ---> GND
  | Pin 28 (RD5) ---> [470R] ---> [LED5] ---> GND
  | Pin 29 (RD6) ---> [470R] ---> [LED6] ---> GND
  | Pin 30 (RD7) ---> [470R] ---> [LED7] ---> GND
  +-----------------------------+
```

---

## 6. Algorithm (Section 5, Page 2.9)
1. **Activate internal pull-up resistors** on PORTB (`INTCON2bits.RBPU = 0`).
2. **Disable all analog inputs** by setting `ADCON1 = 0x0F` (all pins configured as digital I/O).
3. **Configure input pins:** Configure `RB0` and `RB1` as inputs (`TRISBbits.TRISB0 = 1; TRISBbits.TRISB1 = 1`) to sense `SW1` and `SW0`.
4. **Configure output pins:**
   - Configure `RC1` (Relay) as output (`TRISCbits.TRISC1 = 0`).
   - Configure `RC2` (Buzzer) as output (`TRISCbits.TRISC2 = 0`).
   - Configure `PORTD` (8x LEDs) as output (`TRISD = 0x00`).
5. **Initialize outputs:** Keep LEDs, Buzzer, and Relay OFF on Power-ON / Reset (`PORTD = 0x00; LATCbits.LATC1 = 0; LATCbits.LATC2 = 0`).
6. **Check status of RB1 (SW0 press):**
   - When pressed (`PORTBbits.RB1 == 0`), turn ON the Relay (`LATC1 = 1`) and Buzzer (`LATC2 = 1`).
   - Chase LEDs from left to right on `PORTD` (from `0x01` shifting left to `0x80`).
7. **Check status of RB0 (SW1 press):**
   - When pressed (`PORTBbits.RB0 == 0`), turn OFF the Relay (`LATC1 = 0`) and Buzzer (`LATC2 = 0`).
   - Chase LEDs from right to left on `PORTD` (from `0x80` shifting right to `0x01`).

---

## 7. Build Instructions
```powershell
# Compile C source
mcc18.exe -p=18F4550 "linker\exp2.c" -fo="exp2.o"

# Link using relocated linker script
mplink.exe /p18F4550 "linker\rm18f4550-HID-Bootload.lkr" "exp2.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_2.cof" /M"exp_2.map" /W
```
