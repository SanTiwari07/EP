/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 4
 * TITLE: Generation of PWM Signal
 *
 * PROBLEM STATEMENT:
 * Design and develop a PWM based motor speed control system using CCP PWM
 * mode using PIC18F4550 uC.
 *
 * OBJECTIVES:
 * a. To understand the working of PWM.
 * b. To study the on-chip CCP Module (PWM section) of PIC Microcontroller.
 * c. To interface DC motor, via Driver IC L293D, to PIC Microcontroller.
 * d. Apply the PWM signal to control the speed of DC Motor.
 *
 * S/W PACKAGES AND H/W USED:
 * MPLAB IDE, C18 Compiler, Universal Board of PIC18F and DC Motor
 *
 * REFERENCES:
 * a. Mazidi, PIC microcontroller & embedded system 3rd Edition, Pearson
 * b. Datasheet of PIC18F4550 / PIC18F4520 / PIC18F458, www.microchip.com
 * c. Datasheet of L293D, www.ti.com
 *
 * ----------------------------------------------------------------------------
 * PIN CONNECTIONS (INTERFACING DIAGRAM):
 * ----------------------------------------------------------------------------
 * PIC18F4550 Pin            L293D Motor Driver Pin           DC Motor
 * -----------------         ----------------------           --------
 * RC2/CCP1 (Pin 17)  --->   Pin 1 (1,2EN) - PWM Speed Enable
 * RB0 (Pin 33)       --->   Pin 2 (1A)    - Direction IN1
 * RB1 (Pin 34)       --->   Pin 7 (2A)    - Direction IN2
 *                           Pin 3 (1Y)    ----------------> Terminal 1
 *                           Pin 6 (2Y)    ----------------> Terminal 2
 *                           Pins 4,5,12,13 -> GND
 *                           Pin 16 (VCC1)  -> +5V (Logic Supply)
 *                           Pin 8 (VCC2)   -> +12V/+5V (Motor Supply)
 *
 * ----------------------------------------------------------------------------
 * 5. CALCULATION FOR PWM OPERATION (Section 5, Page 4.11):
 * ----------------------------------------------------------------------------
 * Given Parameters:
 *   Oscillator Frequency (Fosc) = 48 MHz
 *   Target PWM Frequency (Fpwm) = 4 kHz (4000 Hz)
 *   Tosc = 1 / Fosc = 1 / 48 MHz = 0.020833 us (20.833 ns)
 *   PWM Period (Tpwm) = 1 / Fpwm = 1 / 4000 = 250 us
 *
 * (A) Finding PR2 Register Value:
 *   PR2 = [ Fosc / (4 * Fpwm * (TMR2 Prescaler)) ] - 1
 *   Using Timer 2 Prescaler = 16:
 *   PR2 = [ 48,000,000 / (4 * 4000 * 16) ] - 1
 *       = [ 48,000,000 / 256,000 ] - 1
 *       = 187.5 - 1 = 186.5
 *   Selecting PR2 = 186 (gives Tpwm = 249.33 us, Fpwm = 4.01 kHz ~ 4 kHz)
 *   (Note: PR2 = 187 can also be used, yielding Fpwm = 3.989 kHz ~ 4 kHz).
 *
 * (B) 10-Bit PWM Duty Cycle Calculation:
 *   CCPR1L:CCP1CON<5:4> = [ (%DCpwm * Fosc) / (100 * Fpwm * (TMR2 Prescaler)) ]
 *                       = %DCpwm * [ 48,000,000 / (100 * 4000 * 16) ]
 *                       = %DCpwm * [ 48,000,000 / 6,400,000 ]
 *                       = %DCpwm * 7.5
 *
 * ----------------------------------------------------------------------------
 * CALCULATION TABLE (Page 4.11):
 * ----------------------------------------------------------------------------
 * Duty Cycle | 10-Bit Value | CCPR1L (Upper 8 bits) | CCP1CON<5:4> (DC1B1:DC1B0)
 * -----------|--------------|-----------------------|---------------------------
 *   20%      |  20 * 7.5=150| 150 >> 2 = 37 (0x25)  | 150 & 3 = 2 (0b10)
 *   40%      |  40 * 7.5=300| 300 >> 2 = 75 (0x4B)  | 300 & 3 = 0 (0b00)
 *   60%      |  60 * 7.5=450| 450 >> 2 = 112(0x70)  | 450 & 3 = 2 (0b10)
 *   80%      |  80 * 7.5=600| 600 >> 2 = 150(0x96)  | 600 & 3 = 0 (0b00)
 *  100%      | 100 * 7.5=750| 750 >> 2 = 187(0xBB)  | 750 & 3 = 2 (0b10)
 * ============================================================================
 */

#include <p18f4550.h>

#define PR2_VALUE 186  /* PR2 value for 4 kHz PWM with 1:16 prescaler at 48 MHz */

extern void _startup(void);

/* Relocate Reset Vector to 0x1000 for USB Bootloader compatibility */
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

/* Delay function calibrated for 48 MHz clock (~1 ms per count) */
void delay(unsigned int time)
{
    unsigned int i, j;

    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 710; j++);
    }
}

/*
 * ----------------------------------------------------------------------------
 * Function to set 10-Bit PWM Duty Cycle:
 * - Upper 8 bits are loaded into CCPR1L
 * - Lower 2 bits are loaded into CCP1CON<5:4> (DC1B1:DC1B0)
 * - PWM mode bits CCP1CON<3:0> = 1100 (0x0C) are preserved
 * ----------------------------------------------------------------------------
 */
void set_pwm_duty_10bit(unsigned int duty_10bit)
{
    /* Upper 8 bits loaded into CCPR1L */
    CCPR1L = (unsigned char)(duty_10bit >> 2);

    /* Lower 2 bits loaded into CCP1CON<5:4> */
    CCP1CONbits.DC1B1 = (duty_10bit >> 1) & 0x01;
    CCP1CONbits.DC1B0 = duty_10bit & 0x01;
}

/*
 * ----------------------------------------------------------------------------
 * Main Function - PWM DC Motor Speed Control
 * ----------------------------------------------------------------------------
 */
void main(void)
{
    /* Look-up table for exact 10-bit duty cycle values as per Table 5 */
    /* Index: 0 -> 20%, 1 -> 40%, 2 -> 60%, 3 -> 80%, 4 -> 100% */
    unsigned int duty_steps[5] = {150, 300, 450, 600, 750};
    int i;

    /* Set all analog pins to digital I/O mode */
    ADCON1 = 0x0F;

    /* Configure motor direction control pins (RB0 and RB1) as output */
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    /* Configure RC2 / CCP1 pin as digital output for PWM signal */
    TRISCbits.TRISC2 = 0;

    /* Set motor direction to Forward: 1A (RB0) = 1, 2A (RB1) = 0 */
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 0;

    /*
     * Step 1: Set PWM period using PR2 register
     * PR2 = 186 gives 4 kHz PWM frequency with 1:16 prescaler at Fosc = 48 MHz
     */
    PR2 = PR2_VALUE;

    /*
     * Step 2: Configure CCP1CON register for PWM mode
     * Bits 3-0 = 1100 (PWM mode)
     * Bits 5-4 = 00   (Initial LSBs of duty cycle = 0)
     */
    CCP1CON = 0x0C;

    /*
     * Step 3: Set initial duty cycle to 0 (Motor stopped initially)
     */
    set_pwm_duty_10bit(0);

    /*
     * Step 4: Configure Timer 2
     * T2CON = 0x06:
     *   Bit 2 (TMR2ON)    = 1 (Timer 2 is ON)
     *   Bits 1-0 (T2CKPS) = 10 (Timer 2 Prescaler is 1:16)
     *   Bits 6-3 (T2OUTPS)= 0000 (Postscaler 1:1)
     */
    T2CON = 0x06;

    while (1)
    {
        /*
         * Gradually increase motor speed through calculated duty cycle steps:
         * 20% (150) -> 40% (300) -> 60% (450) -> 80% (600) -> 100% (750)
         */
        for (i = 0; i < 5; i++)
        {
            set_pwm_duty_10bit(duty_steps[i]);
            delay(1500);  /* Hold each speed for 1.5 seconds to observe */
        }

        /*
         * Gradually decrease motor speed through calculated duty cycle steps:
         * 100% (750) -> 80% (600) -> 60% (450) -> 40% (300) -> 20% (150)
         */
        for (i = 4; i >= 0; i--)
        {
            set_pwm_duty_10bit(duty_steps[i]);
            delay(1500);  /* Hold each speed for 1.5 seconds to observe */
        }

        /* Stop motor briefly before next ramp cycle */
        set_pwm_duty_10bit(0);
        delay(1000);
    }
}
