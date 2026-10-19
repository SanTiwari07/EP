# Experiment 5: Square Wave / PORTD Toggling Using Timer 0 Polling (`todelay`)

**Course:** Embedded Processors Laboratory (EPL) [2625PC32]  
**Class:** T.Y. B.Tech (Electronics & Telecommunication Engineering)  
**Academic Year:** 2026–27  
**Department:** Department of Electronics & Telecommunication Engineering  
**Institution:** Pune Institute of Computer Technology (PICT), Pune – 411043  

---

## 1. Overview
This program (`exp5.c`) toggles `PORTD` between `0x00` and `0xFF` using a Timer 0 hardware delay function `todelay()` in polling mode on the PIC18F4550 microcontroller.

---

## 2. Register Configuration in `todelay()`
- **`T0CON = 0x08`:**
  - `TMR0ON = 0` (Timer0 initially stopped)
  - `T08BIT = 0` (16-bit timer mode)
  - `T0CS = 0` (Internal instruction cycle clock $F_{osc}/4$)
  - `PSA = 1` (Timer0 prescaler is NOT assigned / bypassed, 1:1)
- **`TMR0H = 0xFB`, `TMR0L = 0x4C`:**
  - Preloads 16-bit Timer 0 register with `0xFB4C` ($64,332_{10}$).
- **Polling Sequence:**
  1. Start Timer 0 (`T0CONbits.TMR0ON = 1`).
  2. Wait until overflow flag sets (`while (INTCONbits.TMR0IF == 0);`).
  3. Stop Timer 0 (`T0CONbits.TMR0ON = 0`).
  4. Clear overflow flag (`INTCONbits.TMR0IF = 0`).

---

## 3. Build Instructions

### Using MPLAB C18 Toolchain
```powershell
# Compile C source
mcc18.exe -p=18F4550 "exp5.c" -fo="exp5.o"

# Link using relocated linker script
mplink.exe /p18F4550 "..\rm18f4550.lkr" "exp5.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"exp_5.cof" /M"exp_5.map" /W
```
