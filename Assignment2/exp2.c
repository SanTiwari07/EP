/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 2
 * TITLE: Interfacing Push buttons, LEDs, Relay & Buzzer to PIC Microcontroller
 *
 * PROBLEM STATEMENT:
 * Interface Push buttons, LEDs, Relay and Buzzer to PIC Microcontroller.
 * Write a program in Embedded C to interact with peripherals as follows:
 * a. LEDs start chasing from left to right and turn ON Relay and Buzzer
 *    whenever pushbutton 1 (Switch SW0 / RB1) is pressed.
 * b. LEDs start chasing from right to left and turn OFF Relay and Buzzer
 *    whenever pushbutton 2 (Switch SW1 / RB0) is pressed.
 *
 * OBJECTIVES:
 * a. To understand the PORT Structure of PIC Microcontroller.
 * b. To study the SFRs to control the PORT Pins (TRIS, PORT, LAT).
 * c. To interface common peripherals like pushbuttons, LEDs, relay, buzzer.
 * d. To understand the use of MPLAB IDE and C18 Compiler.
 * e. To write a simple program in Embedded C.
 *
 * S/W PACKAGES AND H/W USED:
 * MPLAB IDE v8.92, MPLAB C18 Compiler v3.47, Explore PIC Development Board
 *
 * ----------------------------------------------------------------------------
 * PIN CONNECTIONS (INTERFACING DIAGRAM, Page 2.8):
 * ----------------------------------------------------------------------------
 * Peripheral           PIC18F4550 Pin   Function / Notes
 * ----------           --------------   ----------------
 * Push Button SW0      RB1 (Pin 34)     Input with 10k pull-up & internal RBPU
 * Push Button SW1      RB0 (Pin 33)     Input with 10k pull-up & internal RBPU
 * Relay Driver         RC1 (Pin 16)     Output to NPN base (1k) + flyback diode
 * Buzzer Driver        RC2 (Pin 17)     Output to NPN base (1k R9) + Buzzer
 * 8x LEDs              RD0 - RD7        Outputs connected via 470 Ohm resistors
 *
 * ----------------------------------------------------------------------------
 * ALGORITHM (Section 5, Page 2.9):
 * ----------------------------------------------------------------------------
 * 1. Activate the internal pull-up on PORTB (INTCON2bits.RBPU = 0).
 * 2. Disable all analog inputs (ADCON1 = 0x0F) to configure pins as digital I/O.
 * 3. Configure RB0 and RB1 as inputs for sensing SW1 and SW0 respectively.
 * 4. Configure:
 *    a. RC1 (Relay) as output (TRISCbits.TRISC1 = 0).
 *    b. RC2 (Buzzer) as output (TRISCbits.TRISC2 = 0).
 *    c. PORTD (LEDs) as output (TRISD = 0x00).
 * 5. Initialize value for LEDs, Buzzer, and Relay (Keep all OFF on reset).
 * 6. Check status of RB1 (SW0 press):
 *    - Turn ON Relay (LATC1 = 1) and Buzzer (LATC2 = 1).
 *    - Chase LEDs on PORTD from left to right (RD0 -> RD7).
 * 7. Check status of RB0 (SW1 press):
 *    - Turn OFF Relay (LATC1 = 0) and Buzzer (LATC2 = 0).
 *    - Chase LEDs on PORTD from right to left (RD7 -> RD0).
 * ============================================================================
 */

#include <p18f4550.h>
#include "vector_relocate.h"

/* Software delay function calibrated for 48 MHz clock (~1 ms per count) */
void delay(unsigned int time)
{
    unsigned int i, j;

    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 710; j++);
    }
}

void main(void)
{
    /* 2. Disable all analog inputs (all pins digital I/O) */
    ADCON1 = 0x0F;

    /* 1. Activate internal weak pull-up resistors on PORTB */
    INTCON2bits.RBPU = 0;

    /* 3. Configure RB0 (SW1) and RB1 (SW0) as digital inputs */
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;

    /* 4a, 4b. Configure RC1 (Relay) and RC2 (Buzzer) as digital outputs */
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

    /* 4c. Configure PORTD (8x LEDs) as digital output */
    TRISD = 0x00;

    /* 5. Initialize outputs: Keep LEDs, Buzzer, and Relay OFF on startup */
    PORTD = 0x00;
    LATCbits.LATC1 = 0;   /* Relay OFF */
    LATCbits.LATC2 = 0;   /* Buzzer OFF */

    while (1)
    {
        /*
         * 6. Check status of RB1 for Switch SW0 press (active-low: logic 0).
         *    Turn ON Relay & Buzzer; chase LEDs from left to right (RD0 -> RD7).
         */
        if (PORTBbits.RB1 == 0)
        {
            LATCbits.LATC1 = 1;   /* Turn ON Relay */
            LATCbits.LATC2 = 1;   /* Turn ON Buzzer */

            /* Chase LEDs from left to right: RD0 (0x01) up to RD7 (0x80) */
            PORTD = 0x01;
            while (PORTD != 0x80)
            {
                delay(250);
                PORTD = PORTD << 1;
            }
            delay(250);
        }

        /*
         * 7. Check status of RB0 for Switch SW1 press (active-low: logic 0).
         *    Turn OFF Relay & Buzzer; chase LEDs from right to left (RD7 -> RD0).
         */
        if (PORTBbits.RB0 == 0)
        {
            LATCbits.LATC1 = 0;   /* Turn OFF Relay */
            LATCbits.LATC2 = 0;   /* Turn OFF Buzzer */

            /* Chase LEDs from right to left: RD7 (0x80) down to RD0 (0x01) */
            PORTD = 0x80;
            while (PORTD != 0x01)
            {
                delay(250);
                PORTD = PORTD >> 1;
            }
            delay(250);
        }
    }
}