/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 1 (Part D)
 * TITLE: Interfacing LEDs with 89C51 - LED Chasing (Left & Right)
 *
 * PROBLEM STATEMENT:
 * D. Write a program in C language for LED Chasing.
 *
 * OBJECTIVES:
 * a. To study the Port structure of 8051 Microcontroller.
 * b. To study LED interfacing with bit-shifting operations.
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
 * Machine Cycle Period (MC) = 12 / 11.0592 MHz = 1.085 us
 * Calibrated inner loop of 1275 cycles provides ~1 ms per count.
 *
 * ----------------------------------------------------------------------------
 * ALGORITHM 2.4: LED Chasing (Left / Right, Page 1.4):
 * ----------------------------------------------------------------------------
 * 1. Start.
 * 2. Initialize the Port pins with 0x01 for left shift and 0x80 for right shift.
 * 3. Call delay subroutine of 100 ms.
 * 4. Shift the logic written on port pins towards left side by one bit position
 *    in a loop for 8 times.
 * 5. Call delay subroutine of 100 ms.
 * 6. Repeat for right shift, and repeat continuously in an infinite loop.
 * ============================================================================
 */

#include <reg51.h>

/* Delay function calibrated for 11.0592 MHz clock (~1 ms per count) */
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
    unsigned int i;

    while (1)
    {
        /* --- Left Shift Chasing Sequence (P1.0 to P1.7) --- */
        P1 = 0x01;
        for (i = 0; i < 8; i++)
        {
            delay(100);       /* 100 ms display time per LED */
            P1 = P1 << 1;     /* Shift bit leftwards */
        }

        /* --- Right Shift Chasing Sequence (P1.7 to P1.0) --- */
        P1 = 0x80;
        for (i = 0; i < 8; i++)
        {
            delay(100);       /* 100 ms display time per LED */
            P1 = P1 >> 1;     /* Shift bit rightwards */
        }
    }
}