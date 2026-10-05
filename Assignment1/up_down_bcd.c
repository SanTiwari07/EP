/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 1 (Part C)
 * TITLE: Interfacing LEDs with 89C51 - Two-Digit BCD Up/Down Counter
 *
 * PROBLEM STATEMENT:
 * C. Write a program in C language for up/down counting of BCD numbers
 *    up to two digits. Display the result on LEDs connected to PORT P1.
 *
 * OBJECTIVES:
 * a. To study the Port structure of 8051 Microcontroller.
 * b. To understand Packed Binary Coded Decimal (BCD) number format.
 * c. To study delay generation using looping.
 * d. To study two-digit BCD counting on 8 LEDs.
 *
 * S/W PACKAGES AND H/W USED:
 * Keil uVision IDE, Windows 10, AT89C51 / AT89S52 Universal Trainer Board
 *
 * ----------------------------------------------------------------------------
 * THEORY & BCD NUMBER REPRESENTATION (Section 1.3, Page 1.3):
 * ----------------------------------------------------------------------------
 * In packed BCD representation:
 * - Upper 4 bits represent the tens digit (0-9).
 * - Lower 4 bits represent the units digit (0-9).
 * - Format: P1 = (tens << 4) | units.
 * - Valid values range from 0x00 (00) to 0x99 (99). Non-decimal hex digits
 *   (A through F) are excluded.
 *
 * ----------------------------------------------------------------------------
 * ALGORITHM 2.3: Up/Down Counting of BCD Numbers (Page 1.4):
 * ----------------------------------------------------------------------------
 * 1. Start.
 * 2. Set tens digit and units digit to 0.
 * 3. Pack BCD into 8-bit value: (tens << 4) | units.
 * 4. Send packed BCD number to PORT P1.
 * 5. Call delay subroutine of 100 ms.
 * 6. Increment units from 0 to 9; when units exceeds 9, reset to 0 and
 *    increment tens from 0 to 9 (BCD Up Count: 00 to 99).
 * 7. Reverse sequence from 99 down to 00 (BCD Down Count).
 * 8. Repeat continuously in an infinite loop.
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
    int tens, units;

    while (1)
    {
        /* --- Two-Digit BCD UP Counting: 00 to 99 --- */
        for (tens = 0; tens <= 9; tens++)
        {
            for (units = 0; units <= 9; units++)
            {
                P1 = (tens << 4) | units;  /* Pack tens & units into P1 */
                delay(100);                /* 100 ms display delay */
            }
        }

        /* --- Two-Digit BCD DOWN Counting: 99 down to 00 --- */
        for (tens = 9; tens >= 0; tens--)
        {
            for (units = 9; units >= 0; units--)
            {
                P1 = (tens << 4) | units;  /* Pack tens & units into P1 */
                delay(100);                /* 100 ms display delay */
            }
        }
    }
}