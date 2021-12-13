/*
 * PWMDriver.c
 *
 */

#include "PWMDriver.h"

/*
 *  pwm_gpio_Init
 * 
 *  @brief Initialize GPIO pin P4.1 to output PWM signal
 *
 *  @param
 *  @return
 */
static void pwm_gpio_Init( void );

/*
 *  pwm_Init
 */
void pwm_Init( void )
{
    // Initialize gpio pins
    pwm_gpio_Init();

    // Halt Timer
    TA3CTL &= ~(MC);

    // Init Timer with SMCLK /4 prescaler
    TA3CTL |= TASSEL_2 | ID_2 | MC_0;

    // Init /5 prescaler, 1 step = 1us
    TA3EX0 |= TAIDEX_4;

    // Set timer overflow to 20ms
    TA3CCR0 = 20000;

    // Set the output to be Reset/Set
    TA3CCTL1 |= OUTMOD_3;
    TA3CCTL2 |= OUTMOD_3;

    // Set duty cycle to 50%
    set_Duty(10000);

    // Reset timer and start the timer
    TA3CTL |= TACLR;
    TA3CTL |= MC_1;
}

/*
 *  set_Duty
 */
void set_Duty( uint16_t duty_cycle )
{
    // Set the compare register for the duty cycle
    TA3CCR1 = 10000;
    TA3CCR2 = 5000;
}

/*
 *  pwm_gpio_Init
 */
static void pwm_gpio_Init( void )
{
    // Change to the secondary port
    SYSCFG3 |= (TA3RMP);

    // Set Timer pin to output
    P4DIR |= (BIT6);

    // Set the pin operation to Timer out
    P4SEL1 |= (BIT6);
    P4SEL0 &= ~(BIT6);

    // Set Timer pin to output
    P4DIR |= (BIT5);

    // Set the pin operation to Timer out
    P4SEL1 |= (BIT5);
    P4SEL0 &= ~(BIT5);
}
