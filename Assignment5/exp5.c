/*
 * ============================================================================
 * PUNE INSTITUTE OF COMPUTER TECHNOLOGY, PUNE - 411043
 * Department of Electronics & Telecommunication Engineering
 * Course: EPL (Embedded Processors Laboratory) | Class: T.Y. E&TC Engg.
 *
 * EXPT. NO.: 5
 * TITLE: LCD Interfacing with PIC18F4550
 *
 * PROBLEM STATEMENT:
 * Interface 16x2 LCD module to PIC microcontroller and write a program in
 * Embedded C to display characters on 16x2 LCD display.
 *
 * OBJECTIVES:
 * a. To understand the working of LCD module.
 * b. To study the LCD section of PIC Microcontroller.
 * c. To interface LCD module to PIC Microcontroller (HD44780 8-bit interface).
 *
 * S/W PACKAGES AND H/W USED:
 * MPLAB IDE, C18 Compiler, PICkit3 / USB HID Bootloader, Universal Development Board
 *
 * REFERENCES:
 * a. Mazidi, PIC microcontroller & embedded system 3rd Edition, Pearson
 * b. Datasheet of PIC18F4550 / PIC18F4520 / PIC18F458, www.microchip.com
 *
 * ----------------------------------------------------------------------------
 * PIN CONNECTIONS (INTERFACING DIAGRAM, Page 9.4):
 * ----------------------------------------------------------------------------
 * PIC18F4550 Pin            16x2 LCD Module Pin          Description
 * -----------------         -------------------          -----------
 * RE0 (Pin 8)        --->   RS (Pin 4)                   Register Select (0=Cmd, 1=Data)
 * RE1 (Pin 9)        --->   RW (Pin 5)                   Read / Write (0=Write, 1=Read)
 * RE2 (Pin 10)       --->   EN (Pin 6)                   Enable (Latch pulse)
 * RD0 - RD7 (Pins 19-30) -> D0 - D7 (Pins 7 - 14)        8-Bit Bidirectional Data Bus
 * GND                --->   VSS (Pin 1)                  Power Ground
 * +5V                --->   VDD (Pin 2)                  +5V Power Supply
 * 10k Pot Wiper      --->   VEE (Pin 3)                  Contrast Adjustment
 * +5V                --->   A / LED+ (Pin 15)            Backlight Anode
 * GND                --->   K / LED- (Pin 16)            Backlight Cathode
 *
 * ----------------------------------------------------------------------------
 * LCD COMMAND SUMMARY:
 * ----------------------------------------------------------------------------
 * 0x38: Function Set: 2 lines, 5x7 matrix, 8-bit mode
 * 0x0E: Display ON, cursor ON (or 0x0C: Display ON, cursor OFF)
 * 0x01: Clear LCD display screen
 * 0x06: Entry Mode: Auto-increment cursor position to right
 * 0x80: Force cursor to beginning of 1st row (0xC0 for 2nd row)
 * ============================================================================
 */

#include <p18f4550.h>

#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define EN PORTEbits.RE2

/* Function Prototypes */
void delay(unsigned int time);
void Lcd_cmd(unsigned char command);
void Lcd_init(void);
void Lcd_data(unsigned char data);
void Lcd_string(const rom char *str);

extern void _startup(void);

/* Relocate Reset Vector to 0x1000 for USB Bootloader compatibility */
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

/* Delay routine calibrated for 48 MHz system clock */
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
 * Send Command byte to 16x2 LCD
 * Steps (Page 9.3):
 * 1. Place command on PORTD (D0 - D7)
 * 2. Make RS = 0 (Command register select)
 * 3. Make RW = 0 (Write operation)
 * 4. Generate high-to-low Enable pulse (EN = 1 then EN = 0)
 * 5. Call delay
 * ----------------------------------------------------------------------------
 */
void Lcd_cmd(unsigned char command)
{
    PORTD = command;

    RS = 0;   /* Command register select */
    RW = 0;   /* Write operation */
    EN = 1;
    delay(5);
    EN = 0;
    delay(5);
}

/*
 * ----------------------------------------------------------------------------
 * Initialize 16x2 LCD in 8-bit mode
 * ----------------------------------------------------------------------------
 */
void Lcd_init(void)
{
    delay(15);         /* Wait for LCD power-up stabilization */
    Lcd_cmd(0x38);     /* 8-bit mode, 2 lines, 5x7 dots */
    delay(5);
    Lcd_cmd(0x01);     /* Clear display */
    delay(5);
    Lcd_cmd(0x0E);     /* Display ON, cursor ON */
    delay(5);
    Lcd_cmd(0x06);     /* Entry mode: increment cursor */
    delay(5);
    Lcd_cmd(0x80);     /* Set cursor to line 1, column 1 */
    delay(5);
}

/*
 * ----------------------------------------------------------------------------
 * Send Character (ASCII Data) byte to 16x2 LCD
 * Steps (Page 9.3):
 * 1. Place data on PORTD (D0 - D7)
 * 2. Make RS = 1 (Data register select)
 * 3. Make RW = 0 (Write operation)
 * 4. Generate high-to-low Enable pulse (EN = 1 then EN = 0)
 * 5. Call delay
 * ----------------------------------------------------------------------------
 */
void Lcd_data(unsigned char data)
{
    PORTD = data;

    RS = 1;   /* Data register select */
    RW = 0;   /* Write operation */
    EN = 1;
    delay(5);
    EN = 0;
    delay(5);
}

/*
 * ----------------------------------------------------------------------------
 * Send null-terminated string to LCD
 * ----------------------------------------------------------------------------
 */
void Lcd_string(const rom char *str)
{
    while (*str)
    {
        Lcd_data(*str);
        str++;
    }
}

void main(void)
{
    /* Set all analog pins to digital I/O mode */
    ADCON1 = 0x0F;

    /* Configure PORTD (Data bus D0-D7) and PORTE (Control RS, RW, EN) as outputs */
    TRISD = 0x00;
    TRISE = 0x00;

    /* Initialize LCD */
    Lcd_init();

    /* Display "HELLO WORLD" on Line 1 */
    Lcd_cmd(0x80);  /* Cursor to Line 1 */
    Lcd_string("HELLO WORLD");

    /* Infinite loop - Display remains steady */
    while (1)
    {
    }
}