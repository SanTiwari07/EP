/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 3
 * TITLE: Generation of Square wave using Timer 0 interrupt.
 *
 * PROBLEM STATEMENT:
 * Implement Timer 0 interrupt concept using PIC18F4550 uC to generate a
 * square wave of 10 Hz frequency.
 *
 * OBJECTIVES:
 * a. To understand the basic concepts of Timer and Counter.
 * b. To study in detail Timer 0 of PIC Microcontroller.
 * c. To study interrupt structure of PIC Microcontroller.
 * d. To use timer interrupt and its related SFRs.
 *
 * S/W PACKAGES AND H/W USED:
 * MPLAB IDE, C18 Compiler, DSO, Universal Board of PIC18F and PICkit3
 *
 * REFERENCES:
 * a. Mazidi, PIC microcontroller & embedded system 3rd Edition, Pearson
 * b. Datasheet of PIC18F4550 / PIC18F4520 / PIC18F458, www.microchip.com
 *
 * ----------------------------------------------------------------------------
 * CALCULATION FOR GENERATING SQUARE WAVE OF 10 Hz (Section 5.2):
 * ----------------------------------------------------------------------------
 * Desired Wave Frequency (F) = 10 Hz
 * Desired Time Period (T) = 1 / F = 1 / 10 Hz = 0.1 s = 100 ms = 100,000 us
 * Half-cycle time (delay per toggle, Td) = T / 2 = 50 ms = 50,000 us
 *
 * Oscillator Frequency (Fosc) = 48 MHz (Universal Board PIC18F4550)
 * Internal Instruction Clock (Fcy) = Fosc / 4 = 48 MHz / 4 = 12 MHz
 * Timer Period (Tp) = 1 / Fcy = 4 / Fosc = 1 / 12 MHz = 0.08333 us (1/12 us)
 *
 * Using Timer0 in 16-bit Mode with Prescaler = 1:16 (T0CON<2:0> = 011):
 * Equivalent Timer Period (Teq) = Tp * Prescaler = (1/12 us) * 16 = 4/3 us
 * Number of counts (n) = Td / Teq = 50,000 us / (4/3 us)
 *                      = (50,000 * 3) / 4 = 37,500 counts
 *
 * Initial 16-bit Register Count = 65,536 - n
 *                               = 65,536 - 37,500 = 28,036
 *
 * Converting 28,036 to Hexadecimal:
 * 28,036 / 256 = 109 with remainder 132
 * 109 = 0x6D (TMR0H)
 * 132 = 0x84 (TMR0L)
 * Initial hex value = 0x6D84
 *   TMR0H = 0x6D
 *   TMR0L = 0x84
 *
 * ----------------------------------------------------------------------------
 * T0CON (Timer0 Control Register) Configuration:
 * ----------------------------------------------------------------------------
 * Bit 7: TMR0ON = 0 (Timer stopped during configuration, enabled later)
 * Bit 6: T08BIT = 0 (16-bit timer/counter mode)
 * Bit 5: T0CS   = 0 (Internal instruction cycle clock CLKO / Fosc/4)
 * Bit 4: T0SE   = 0 (Increment on low-to-high transition)
 * Bit 3: PSA    = 0 (Timer0 prescaler is assigned)
 * Bits 2-0: TOPS2:TOPS0 = 011 (Prescaler 1:16)
 *
 * T0CON = 0b00000011 = 0x03 (Note: When timer is ON, T0CON = 0x83)
 * Note: Algorithm step 2 refers to 16-bit mode, internal clock, 1:16 prescaler.
 * ============================================================================
 */

#include <p18f4550.h>

/* Function prototypes */
void timer0_isr(void);

extern void _startup(void);

/* Relocate Reset Vector to 0x1000 for USB HID Bootloader compatibility */
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

/* Relocate High Priority Interrupt Vector to 0x1008 as specified in Algorithm */
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_vector(void)
{
    _asm goto timer0_isr _endasm
}
#pragma code

/*
 * ----------------------------------------------------------------------------
 * Step 6: Interrupt Service Routine (ISR) for Timer 0
 * ----------------------------------------------------------------------------
 */
#pragma interrupt timer0_isr
void timer0_isr(void)
{
    /* Check if Timer 0 overflow interrupt flag is set */
    if (INTCONbits.TMR0IF == 1)
    {
        /* 6a. Toggle the PORT pin RB0 */
        PORTBbits.RB0 = ~PORTBbits.RB0;

        /* 6b. Clear the TMR0IF flag for the next round */
        INTCONbits.TMR0IF = 0;

        /* 6c. Reload TMR0H first, then TMR0L with calculated count (0x6D84) */
        TMR0H = 0x6D;
        TMR0L = 0x84;
    }
}

/*
 * ----------------------------------------------------------------------------
 * Main Function - Follows Section 7: Algorithm
 * ----------------------------------------------------------------------------
 */
void main(void)
{
    /* Set all ADC pins as digital I/O */
    ADCON1 = 0x0F;

    /* 1. Configure Port pin RB0 as output and initial value 0 */
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    /*
     * 2. Load value in T0CON: 16-bit mode, internal clock source (Fosc/4),
     *    prescaler assigned with 1:16.
     *    T0CON = 0x03 (TMR0ON=0, T08BIT=0, T0CS=0, T0SE=0, PSA=0, TOPS=011)
     */
    T0CON = 0x03;

    /* 3. Load registers TMR0H first and then TMR0L with calculated count */
    TMR0H = 0x6D;
    TMR0L = 0x84;

    /* 4. Enable Timer0 Interrupt and Global Interrupt using INTCON Register */
    INTCONbits.TMR0IF = 0;  /* Clear Timer0 flag */
    INTCONbits.TMR0IE = 1;  /* Enable Timer0 overflow interrupt */
    INTCONbits.GIE = 1;     /* Enable Global Interrupts */

    /* 5. Start the timer by setting TMR0ON bit in T0CON (T0CON becomes 0x83) */
    T0CONbits.TMR0ON = 1;

    /*
     * 7. Idle loop: CPU remains free while interrupt generates 10 Hz square wave.
     *    Result can be verified on DSO / Oscilloscope connected to pin RB0.
     */
    while (1)
    {
        /* Background processing can be added here if needed */
    }
}
