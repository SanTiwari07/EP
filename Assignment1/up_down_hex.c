/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 1 (Part B)
 * TITLE: Interfacing LEDs with 89C51 - Two-Digit Hexadecimal Up/Down Counter
 *
 * PROBLEM STATEMENT:
 * B. Write a program in C language for up/down counting of hex numbers
 *    up to two digits. Display the result on LEDs connected to PORT P1.
 *
 * OBJECTIVES:
 * a. To study the Port structure of 8051 Microcontroller.
 * b. To study hexadecimal representation on 8-LED display.
 * c. To study loop-based delay generation.
 * d. To study up/down counting implementation.
 *
 * S/W PACKAGES AND H/W USED:
 * Keil uVision IDE, Windows 10, AT89C51 / AT89S52 Universal Trainer Board
 *
 * ----------------------------------------------------------------------------
 * THEORY & HEXADECIMAL REPRESENTATION (Section 1.2, Page 1.2):
 * ----------------------------------------------------------------------------
 * An 8-bit port (P1) can represent two hexadecimal digits (0x00 to 0xFF):
 * - Upper nibble (P1.7 - P1.4) displays the Most Significant Digit (0 to F).
 * - Lower nibble (P1.3 - P1.0) displays the Least Significant Digit (0 to F).
 *
 * ----------------------------------------------------------------------------
 * ALGORITHM 2.2: Up/Down Counting of Hex Numbers (Page 1.4):
 * ----------------------------------------------------------------------------
 * 1. Start.
 * 2. Set the counter register value (start at 0x00).
 * 3. Send hexadecimal number to port pins (P1 = i).
 * 4. Call delay subroutine of 100 ms.
 * 5. Increment hexadecimal number until 0xFF (Up Count).
 * 6. Decrement hexadecimal number down to 0x00 (Down Count).
 * 7. Repeat continuously in an infinite loop.
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
    unsigned int count;

    while (1)
    {
        /* --- Hexadecimal UP Counting: 0x00 to 0xFF (0 to 255) --- */
        for (count = 0; count <= 255; count++)
        {
            P1 = count;    /* Output 8-bit hex count on PORT 1 */
            delay(100);    /* 100 ms delay between count states */
        }

        /* --- Hexadecimal DOWN Counting: 0xFF down to 0x00 --- */
        for (count = 255; count > 0; count--)
        {
            P1 = count;    /* Output 8-bit hex count on PORT 1 */
            delay(100);    /* 100 ms delay between count states */
        }

        P1 = 0x00;
        delay(100);
    }
}