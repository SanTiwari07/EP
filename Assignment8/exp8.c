/*
 * Experiment 8: Configure and evaluate STM32 GPIO and timer functionalities
 *               to optimize system performance.
 *
 * Target: STM32 Microcontroller (ARM Cortex-M, e.g. STM32F103 "Blue Pill" / Nucleo)
 * Onboard LED: Pin PC13 (Active LOW on STM32F103 Blue Pill) or PA5 (Nucleo)
 * Timer: General Purpose Timer (TIM2)
 *
 * Objective & Performance Optimization Analysis:
 * ----------------------------------------------
 * 1. GPIO Configuration:
 *    - Enable peripheral clock via RCC (Reset and Clock Control).
 *    - Configure GPIO pin mode as General Purpose Output Push-Pull.
 * 2. Hardware Timer Configuration (TIM2):
 *    - Software delay loops (for/while) waste 100% of CPU cycles, vary with compiler
 *      optimizations, and cause timing jitter.
 *    - Hardware Timer runs independently of the CPU core at the APB bus clock.
 *    - By prescaling the system clock (PSC) and setting Auto-Reload (ARR), we achieve
 *      exact millisecond delays with zero CPU calculation drift.
 *    - Optimization: The CPU is freed from calculating timing loops, allowing power saving
 *      or concurrent multitasking.
 */

#include <stdint.h>

/* Base Addresses for Peripherals on STM32F103 (ARM Cortex-M3) */
#define RCC_BASE        0x40021000UL
#define GPIOC_BASE      0x40011000UL
#define TIM2_BASE       0x40000000UL

/* Register Definitions */
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define GPIOC_CRH       (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))

#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_SR         (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_CNT        (*(volatile uint32_t *)(TIM2_BASE + 0x24))

/* Bit Masks */
#define RCC_APB2ENR_IOPCEN  (1 << 4)   /* Enable GPIOC Clock */
#define RCC_APB1ENR_TIM2EN  (1 << 0)   /* Enable TIM2 Clock */
#define TIM_SR_UIF          (1 << 0)   /* Update Interrupt Flag */
#define TIM_CR1_CEN         (1 << 0)   /* Counter Enable */

/*
 * Function: GPIO_Init
 * Purpose: Enables clock for Port C and configures PC13 as Output Push-Pull (2 MHz)
 */
void GPIO_Init(void)
{
    /* Step 1: Enable clock for GPIOC peripheral */
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

    /*
     * Step 2: Configure PC13 in CRH register:
     * Mode bits [21:20] = 10 (Output mode, max speed 2 MHz)
     * CNF bits  [23:22] = 00 (General purpose output push-pull)
     * Clear bits 20 to 23 first, then set MODE13 to 0b0010
     */
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);

    /* Turn OFF LED initially (PC13 active LOW -> set HIGH to turn OFF) */
    GPIOC_ODR |= (1 << 13);
}

/*
 * Function: Timer2_Init
 * Purpose: Configures Hardware Timer 2 to generate 1 ms timebase
 * Clock: Default Internal RC Oscillator (HSI) = 8 MHz (or APB1 Clock)
 */
void Timer2_Init(void)
{
    /* Step 1: Enable clock for TIM2 peripheral on APB1 bus */
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * Step 2: Set Prescaler (PSC)
     * Timer clock = 8 MHz.
     * With PSC = 7999, Timer Counter Clock = 8 MHz / (7999 + 1) = 1 kHz (1 tick = 1 ms)
     */
    TIM2_PSC = 7999;

    /*
     * Step 3: Set Auto-Reload Register (ARR) to max value (65535)
     */
    TIM2_ARR = 65535;

    /*
     * Step 4: Clear counter and status register
     */
    TIM2_CNT = 0;
    TIM2_SR  = 0;

    /* Step 5: Enable Timer 2 counter */
    TIM2_CR1 |= TIM_CR1_CEN;
}

/*
 * Function: delay_ms
 * Purpose: Generates precise delay in milliseconds using Hardware Timer 2
 * Performance advantage: Accurate timing, immune to CPU pipeline / loop variations
 */
void delay_ms(uint16_t ms)
{
    TIM2_CNT = 0;             /* Reset counter */
    while (TIM2_CNT < ms);    /* Wait until counter reaches target ms */
}

int main(void)
{
    /* Initialize peripherals */
    GPIO_Init();
    Timer2_Init();

    while(1)
    {
        /* Turn ON LED (PC13 = LOW) */
        GPIOC_ODR &= ~(1 << 13);
        delay_ms(500);         /* 500 ms hardware timer delay */

        /* Turn OFF LED (PC13 = HIGH) */
        GPIOC_ODR |= (1 << 13);
        delay_ms(500);         /* 500 ms hardware timer delay */
    }
}
