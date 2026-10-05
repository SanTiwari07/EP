/*
 * Experiment 6: Interface and program a temperature sensor (LM35)
 *               with PIC18F4550 to display real-time readings on a 16x2 LCD.
 *
 * Target Microcontroller: PIC18F4550
 * Sensor: LM35 Temperature Sensor connected to RA0 / AN0 (Pin 2)
 * LCD Connections (Matches Assignment 5):
 *   RS -> RE0
 *   RW -> RE1
 *   EN -> RE2
 *   Data (D0-D7) -> PORTD (RD0 - RD7)
 *
 * Temperature Calculation:
 * ------------------------
 * LM35 Sensitivity = 10 mV / degree C
 * 10-bit ADC, Vref = 5V = 5000 mV
 * Resolution = 5000 mV / 1023 steps ~ 4.887 mV / step
 * Temperature in degree C = (ADC_Value * 4.887 mV) / 10 mV
 *                         = (ADC_Value * 500) / 1023
 */

#include <p18f4550.h>

extern void _startup(void);

/* Relocate Reset Vector for USB Bootloader */
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset(void)
{
    _asm goto _startup _endasm
}
#pragma code

/* LCD Control Pin Definitions */
#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define EN PORTEbits.RE2

/* Function Prototypes */
void delay(unsigned int time);
void Lcd_cmd(unsigned char command);
void Lcd_data(unsigned char data);
void Lcd_init(void);
void Lcd_string(const rom char *str);
unsigned int read_adc(void);

/* Delay function */
void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 180; j++);
    }
}

/* Send command byte to LCD */
void Lcd_cmd(unsigned char command)
{
    PORTD = command;

    RS = 0;
    RW = 0;
    EN = 1;
    delay(20);

    EN = 0;
    delay(20);
}

/* Send data byte (character) to LCD */
void Lcd_data(unsigned char data)
{
    PORTD = data;

    RS = 1;
    RW = 0;
    EN = 1;
    delay(20);

    EN = 0;
    delay(20);
}

/* Initialize 16x2 LCD in 8-bit mode */
void Lcd_init(void)
{
    Lcd_cmd(0x38);  /* 2 lines, 5x7 matrix in 8-bit mode */
    delay(50);

    Lcd_cmd(0x01);  /* Clear display */
    delay(50);

    Lcd_cmd(0x0C);  /* Display ON, cursor OFF */
    delay(50);

    Lcd_cmd(0x06);  /* Increment cursor (shift right) */
    delay(50);

    Lcd_cmd(0x80);  /* Cursor at line 1, position 0 */
    delay(50);
}

/* Send a string literal stored in ROM to LCD */
void Lcd_string(const rom char *str)
{
    while(*str)
    {
        Lcd_data(*str);
        str++;
    }
}

/* Read 10-bit value from ADC Channel AN0 */
unsigned int read_adc(void)
{
    unsigned int adc_val;

    ADCON0bits.GO = 1;             /* Start ADC conversion */
    while(ADCON0bits.GO == 1);     /* Wait for conversion to complete */

    adc_val = ADRESH;
    adc_val = (adc_val << 8) | ADRESL; /* Combine 10-bit result */

    return adc_val;
}

void main(void)
{
    unsigned int adc_val;
    unsigned int temp;
    unsigned char d1, d2, d3;

    /* Configure I/O Pins */
    TRISA = 0x01;    /* RA0/AN0 as input for temperature sensor */
    TRISD = 0x00;    /* PORTD as output for LCD data */
    TRISE = 0x00;    /* PORTE as output for LCD control pins (RE0, RE1, RE2) */

    /*
     * Configure ADC Registers:
     * ADCON1 = 0x0E: AN0 as analog input, all other pins digital, Vref = VDD/VSS
     * ADCON0 = 0x01: Select Channel 0 (AN0), ADC module enabled (ADON = 1)
     * ADCON2 = 0x8A: Right justified result, 2 TAD acquisition, Fosc/32 clock
     */
    ADCON1 = 0x0E;
    ADCON0 = 0x01;
    ADCON2 = 0x8A;

    /* Initialize LCD */
    Lcd_init();

    /* Display static header on Line 1 */
    Lcd_cmd(0x80);
    Lcd_string("TEMP MONITOR");

    while(1)
    {
        /* Read ADC from LM35 */
        adc_val = read_adc();

        /*
         * Convert ADC count to Temperature in Celsius
         * Temp = (ADC * 500) / 1023
         */
        temp = (unsigned int)(((unsigned long)adc_val * 500) / 1023);

        /* Extract digits for hundreds, tens, and units */
        d1 = (temp / 100) + '0';
        d2 = ((temp / 10) % 10) + '0';
        d3 = (temp % 10) + '0';

        /* Move cursor to Line 2 */
        Lcd_cmd(0xC0);
        Lcd_string("Temp: ");

        /* Print only relevant digits */
        if(d1 != '0')
        {
            Lcd_data(d1);
        }
        Lcd_data(d2);
        Lcd_data(d3);

        /* Print degree symbol and unit 'C' */
        Lcd_data(0xDF);  /* Degree symbol on standard HD44780 LCD */
        Lcd_data('C');
        Lcd_string("   ");

        delay(500);  /* Refresh reading every 500 ms */
    }
}
