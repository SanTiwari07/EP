/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 1 (Part A)
 * TITLE: Interfacing LEDs with 89C51 - LED Toggling
 *
 * PROBLEM STATEMENT:
 * A. Write a program in C language to toggle all the LEDs interfaced to
 *    port P1 of 8051 / 89C51 continuously with delay of 100 ms.
 *    Use looping for delay.
 *
 * OBJECTIVES:
 * a. To study the Port structure of 8051 Microcontroller.
 * b. To study LED interfacing.
 * c. To understand the Keil IDE.
 * d. To study delay generation using loop.
 *
 * S/W PACKAGES AND H/W USED:
 * Keil uVision IDE, Windows 10, AT89C51 / AT89S52 Universal Trainer Board
 *
 * ----------------------------------------------------------------------------
 * DELAY CALCULATION (Section 1.4, Page 1.3):
 * ----------------------------------------------------------------------------
 * Crystal Frequency (XTAL) = 11.0592 MHz
 * Oscillator frequency divided by 12 gives machine cycle frequency:
 *   Frequency = 11.0592 MHz / 12 = 921.6 kHz
 * Machine Cycle Period (MC) = 1 / 921.6 kHz = 1.085 us
 *
 * A nested loop calibrated for 1 ms per count:
 *   Inner loop count = 1275 iterations ~ 1 ms delay at 11.0592 MHz
 *   delay(100) produces approximately 100 ms delay.
 *
 * ----------------------------------------------------------------------------
 * PIN CONNECTIONS (INTERFACING DIAGRAM, Page 1.5):
 * ----------------------------------------------------------------------------
 * AT89C51 Pin          Current Limiting Resistor (470 Ohm)        LEDs
 * ------------         -----------------------------------        ----
 * P1.0 (Pin 1)  -----> R0 (470 Ohm) ---------------------------> LED 0
 * P1.1 (Pin 2)  -----> R1 (470 Ohm) ---------------------------> LED 1
 * P1.2 (Pin 3)  -----> R2 (470 Ohm) ---------------------------> LED 2
 * P1.3 (Pin 4)  -----> R3 (470 Ohm) ---------------------------> LED 3
 * P1.4 (Pin 5)  -----> R4 (470 Ohm) ---------------------------> LED 4
 * P1.5 (Pin 6)  -----> R5 (470 Ohm) ---------------------------> LED 5
 * P1.6 (Pin 7)  -----> R6 (470 Ohm) ---------------------------> LED 6
 * P1.7 (Pin 8)  -----> R7 (470 Ohm) ---------------------------> LED 7
 * All LED Anodes are tied to +5V (Vcc) via common pull-up / active-low sink mode.
 *
 * ----------------------------------------------------------------------------
 * ALGORITHM 2.1 (Page 1.4):
 * 1. Start.
 * 2. Send high logic data on port pins (P1 = 0xFF).
 * 3. Call delay subroutine of 100 ms.
 * 4. Send low logic data on port pins (P1 = 0x00).
 * 5. Call delay subroutine of 100 ms.
 * 6. Repeat in an infinite loop.
 * ============================================================================
 */

#include <reg51.h>

/* Delay function calibrated for 11.0592 MHz clock (1 ms per count) */
void delay(unsigned int time)
{
    unsigned int i, j;

    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 1275; j++);
    }
}

void main(void)
{
    while (1)
    {
        /* Send low logic data to port pins (turn ON in active-low configuration) */
        P1 = 0x00;
        delay(100);  /* 100 ms delay */

        /* Send high logic data to port pins (turn OFF in active-low configuration) */
        P1 = 0xFF;
        delay(100);  /* 100 ms delay */
    }
}