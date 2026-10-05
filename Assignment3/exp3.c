/*
 * Experiment 3: Implement Timer 0 Interrupt concept using PIC18F4550
 * to generate a square wave of given frequency (e.g., 1 kHz).
 *
 * Target Microcontroller: PIC18F4550
 * Output Pin: PORTB pin RB0 (or any GPIO pin)
 *
 * Calculation for 1 kHz Square Wave with Fosc = 48 MHz:
 * -----------------------------------------------------
 * Desired Frequency = 1 kHz (Period T = 1 ms = 1000 us)
 * Half-cycle time (delay per toggle) = T / 2 = 500 us
 *
 * Oscillator Frequency (Fosc) = 48 MHz (PIC18F4550 USB Clock)
 * Instruction Clock (Fcy) = Fosc / 4 = 48 MHz / 4 = 12 MHz
 * Instruction Cycle Time (Tcy) = 1 / 12 MHz = 0.0833 us (83.33 ns)
 *
 * Using Timer0 Prescaler = 1:16 (T0CON bits 2:0 = 011):
 * Timer Tick = Tcy * 16 = (1 / 12 us) * 16 = 1.333 us
 * Number of counts needed = 500 us / 1.333 us = 375 counts
 * Initial Count (16-bit) = 65536 - 375 = 65161 = 0xFE89
 * TMR0H = 0xFE
 * TMR0L = 0x89
 */

#include <p18f4550.h>

/* Function prototype */
void timer0_isr(void);

extern void _startup(void);

/* Relocate Reset Vector for USB HID Bootloader */
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

/* Relocate High Priority Interrupt Vector to 0x1008 */
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_vector(void)
{
    _asm goto timer0_isr _endasm
}
#pragma code

/* Timer 0 Interrupt Service Routine (ISR) */
#pragma interrupt timer0_isr
void timer0_isr(void)
{
    /* Check if Timer 0 interrupt flag is set */
    if(INTCONbits.TMR0IF == 1)
    {
        /* Clear the interrupt flag */
        INTCONbits.TMR0IF = 0;

        /* Toggle the output pin to generate square wave */
        PORTBbits.RB0 = ~PORTBbits.RB0;

        /* Reload Timer 0 registers for next half cycle */
        TMR0H = 0xFE;
        TMR0L = 0x89;
    }
}

void main(void)
{
    /* Set all pins as digital I/O */
    ADCON1 = 0x0F;

    /* Configure RB0 as output pin for square wave */
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    /*
     * Configure Timer 0:
     * T0CON = 0x03:
     * Bit 7 (TMR0ON) = 0 (Timer OFF for now)
     * Bit 6 (T08BIT) = 0 (16-bit timer mode)
     * Bit 5 (T0CS)   = 0 (Internal instruction cycle clock Fosc/4)
     * Bit 4 (T0SE)   = 0 (Increment on low-to-high transition)
     * Bit 3 (PSA)    = 0 (Prescaler is assigned)
     * Bits 2-0 (T0PS)= 011 (Prescaler 1:16)
     */
    T0CON = 0x03;

    /* Load initial count for 500 us delay (0xFE89) */
    TMR0H = 0xFE;
    TMR0L = 0x89;

    /* Interrupt Configuration */
    INTCONbits.TMR0IF = 0;  /* Clear Timer 0 interrupt flag */
    INTCONbits.TMR0IE = 1;  /* Enable Timer 0 interrupt */
    INTCONbits.GIE = 1;     /* Enable Global Interrupts */

    /* Turn ON Timer 0 */
    T0CONbits.TMR0ON = 1;

    /* Idle loop: CPU is free while interrupt handles the wave generation */
    while(1)
    {
    }
}
