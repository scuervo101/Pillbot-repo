/*
 * SpeakerDriver.c
 *
 */

#include "SpeakerDriver.h"

/*
 *  speaker_gpio_Init
 * 
 *  @brief Initialize GPIO pin P3.3 to output speaker signal
 *
 *  @param
 *  @return
 */
static void speaker_gpio_Init( void );

/*
 *  speaker_Init
 */
void speaker_Init( void )
{
    // Initialize gpio pins
    speaker_gpio_Init();

    // Halt Timer
    TA2CTL &= ~(MC);

    // Init Timer with SMCLK /4 prescaler
    TA2CTL |= TASSEL_2 | ID_2 | MC_0;

    // Init /5 prescaler, 1 step = 1us
    TA2EX0 |= TAIDEX_4;

    // Set timer overflow to 2ms
    TA2CCR0 = 2000;

    // Set the output mode to reset/set
    TA2CCTL1 |= OUTMOD_3;

    // Set to 50% duty cycle
    TA2CCR1 = 1000;

    // Reset timer and start the timer
    TA3CTL |= TACLR;
}

/*
 *  play_Speaker
 */
void play_Speaker( void )
{
    // Turn on the timer
    TA2CTL |= MC_1; 
}

/*
 *  stop_Speaker
 */
void stop_Speaker( void )
{
    // Turn on the timer
    TA2CTL &= ~(MC_1); 
}

/*
 *  speaker_gpio_Init
 */
static void speaker_gpio_Init( void )
{
    // Set timer pin to output
    P3DIR |= (BIT3);

    // Set the pin operation to Timer out
    P3SEL1 &= ~(BIT3);
    P3SEL0 |= (BIT3);
}