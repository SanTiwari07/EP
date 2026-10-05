/*
 * Experiment 4: Design and develop a PWM-based motor speed control
 *               system using CCP PWM mode on PIC18F4550.
 *
 * Target Microcontroller: PIC18F4550
 * PWM Pin: RC2 / CCP1 (Pin 17) -> Connect to Motor Driver (L293D Enable / PWM input)
 * Direction Pins: RB0 and RB1  -> Connect to Motor Driver (L293D IN1 and IN2)
 *
 * PWM Calculation:
 * ----------------
 * Oscillator Frequency (Fosc) = 12 MHz
 * Timer 2 Prescaler = 16
 *
 * PWM Period = [(PR2) + 1] * 4 * Tosc * TMR2_Prescaler
 * For PR2 = 250 (0xFA):
 * PWM Period = (250 + 1) * 4 * (1 / 12 MHz) * 16 = 1.338 ms (~746 Hz)
 *
 * PWM Duty Cycle:
 * Duty Cycle = CCPR1L * 4 * Tosc * TMR2_Prescaler
 * Setting CCPR1L between 0 (0% speed) and 250 (100% speed) adjusts motor speed.
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

/* Simple delay function */
void delay(unsigned int time)
{
    unsigned int i, j;

    for(i = 0; i < time; i++)
    {
        for(j = 0; j < 710; j++);
    }
}

/* Function to set PWM Duty Cycle */
void set_pwm_duty(unsigned char duty)
{
    CCPR1L = duty;  /* 8-bit duty cycle register */
}

void main(void)
{
    unsigned char speed;

    /* Set all analog pins to digital mode */
    ADCON1 = 0x0F;

    /* Configure motor direction control pins (RB0 and RB1) as output */
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    /* Configure CCP1 / RC2 pin as output for PWM signal */
    TRISCbits.TRISC2 = 0;

    /* Set motor direction: Forward (RB0 = 1, RB1 = 0) */
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 0;

    /*
     * Step 1: Set PWM period using PR2 register
     * PR2 = 250 gives a standard frequency with 1:16 prescaler
     */
    PR2 = 250;

    /*
     * Step 2: Configure CCP1CON register for PWM mode
     * Bits 3-0 = 1100 (PWM mode)
     * Bits 5-4 = 00   (LSBs of duty cycle = 0)
     */
    CCP1CON = 0x0C;

    /*
     * Step 3: Set initial duty cycle to 0 (Motor stopped)
     */
    CCPR1L = 0;

    /*
     * Step 4: Configure Timer 2
     * T2CON = 0x06:
     * Bit 2 (TMR2ON)  = 1 (Timer 2 is ON)
     * Bits 1-0 (T2CKPS) = 10 (Prescaler is 16)
     */
    T2CON = 0x06;

    while(1)
    {
        /*
         * Gradually increase motor speed from 0% to 100%
         * (0 -> 60 -> 120 -> 180 -> 240)
         */
        for(speed = 0; speed <= 240; speed += 60)
        {
            set_pwm_duty(speed);
            delay(500);  /* Hold each speed for 500 ms */
        }

        /*
         * Gradually decrease motor speed from 100% to 0%
         * (240 -> 180 -> 120 -> 60 -> 0)
         */
        for(speed = 240; speed > 0; speed -= 60)
        {
            set_pwm_duty(speed);
            delay(500);  /* Hold each speed for 500 ms */
        }

        set_pwm_duty(0);
        delay(500);
    }
}
