/*
 * USonicDriver.c
 *
 */

#include "USonicDriver.h"

/*
 *  us_enter (uint16_t) Value of timer when echo is triggered
 *  us_exit  (uint16_t) Value of timer after echo is low
 */
static uint16_t us_enter;
static uint16_t us_exit;

/*
 *
 *
 */
static uint8_t usonic;

/*
 *  usonic_Init
 */
void usonic_Init( void )
{
    // Init Pins
    // Set pin 4.3 to output    (TRIG)
    P4DIR |= (BIT3);

    // Set pin 2.4,3.4,4.4,5.4 to input     (ECHO)
    P2DIR &= ~(BIT4);
    P3DIR &= ~(BIT4);
    P4DIR &= ~(BIT4);
    P5DIR &= ~(BIT4);

    // Halt Timer
    TA0CTL &= ~(MC);

    // Init Timer with SMCLK /4 prescaler
    TA0CTL |= TASSEL_2 | ID_2 | MC_0;

    // Init /5 prescaler, 1 step = 1us
    TA0EX0 |= TAIDEX_4;

    // Set timer overflow to 65ms
    TA0CCR0 = 65000;

    // Define cm_range
    us0_range = 0.0f;
    us1_range = 0.0f;
    us2_range = 0.0f;
    us3_range = 0.0f;

    // Start with the first ultrasonic
    usonic = 0;
}

/*
 *  usonic_Start
 */
void usonic_Start( void )
{
    // Enable low to high interrupts for the ECHO pins
    P2IES &= ~(BIT4);
    P3IES &= ~(BIT4);
    P4IES &= ~(BIT4);
    P5IES &= ~(BIT4);

    // Clear the interrupt flag
    P2IFG &= ~(BIT4); 
    P3IFG &= ~(BIT4); 
    P4IFG &= ~(BIT4); 
    P5IFG &= ~(BIT4); 

    // Reset timer module and Enable the timer interrupt
    TA0CTL |= TACLR;
    TA0CTL |= TAIE;

    // Clear the timer interrupt flag
    TA0CTL &= ~(TAIFG);

    // Start the timer
    TA0CTL |= MC_1;
}

// _________ Interrupt Service Routines _________
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    // Clear the interrupt flag
    P2IFG &= ~(BIT4);

    if (P2IN & BIT4) {
        // Change to high-low transition interrupt
        P2IES |= (BIT4);

        // Save the timer when echo transitions to high
        us_enter = TA0R;
    }
    else {
        // Change low-high transition interrupt
        P2IES &= ~(BIT4);

        // Disable this interrupt
        P2IE &= ~(BIT4);

        // Save the timer and solve for the length of the echo pulse
        us_exit = TA0R;
        uint16_t us_range = us_exit - us_enter;

        // Find the distance
        us0_range = us_range/58.0f;
    }
}
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{

    // Clear the interrupt flag
    P3IFG &= ~(BIT4);

    if (P3IN & BIT4) {
        // Change to high-low transition interrupt
        P3IES |= (BIT4);

        // Save the timer when echo transitions to high
        us_enter = TA0R;
    }
    else {
        // Change low-high transition interrupt
        P3IES &= ~(BIT4);

        // Disable this interrupt
        P3IE &= ~(BIT4);

        // Save the timer and solve for the length of the echo pulse
        us_exit = TA0R;
        uint16_t us_range = us_exit - us_enter;

        // Find the distance
        us1_range = us_range/58.0f;
    }
}
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{

    // Clear the interrupt flag
    P4IFG &= ~(BIT4);

    if (P4IN & BIT4) {
        // Change to high-low transition interrupt
        P4IES |= (BIT4);

        // Save the timer when echo transitions to high
        us_enter = TA0R;
    }
    else {
        // Change low-high transition interrupt
        P4IES &= ~(BIT4);

        // Disable this interrupt
        P4IE &= ~(BIT4);

        // Save the timer and solve for the length of the echo pulse
        us_exit = TA0R;
        uint16_t us_range = us_exit - us_enter;

        // Find the distance
        us2_range = us_range/58.0f;
    }
}
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{

    // Clear the interrupt flag
    P5IFG &= ~(BIT4);

    if (P5IN & BIT4) {
        // Change to high-low transition interrupt
        P5IES |= (BIT4);

        // Save the timer when echo transitions to high
        us_enter = TA0R;
    }
    else {
        // Change low-high transition interrupt
        P5IES &= ~(BIT4);

        // Disable this interrupt
        P5IE &= ~(BIT4);

        // Save the timer and solve for the length of the echo pulse
        us_exit = TA0R;
        uint16_t us_range = us_exit - us_enter;

        // Find the distance
        us3_range = us_range/58.0f;
    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerA(void)
{
    // Clear the TAIFG flag
    TA0CTL &= ~(TAIFG);

    // Halt Timer
    TA0CTL &= ~(MC);
    //Reset timer
    TA0CTL |= TACLR;

    // Send the trigger to the ultra sonic
    P4OUT |= BIT3;

    // Start the timer and wait 10us
    TA0CTL |= MC_1;

    // Enable the interrupt for usonic that is being checked
    switch (usonic)
    {
    case 0:             // Checked at delta 0ms
        P2IE |= (BIT4); 
        usonic++;
        break;
    case 1:             // Checked at delta 65ms
        P3IE |= (BIT4); 
        usonic++;
        break;
    case 2:             // Checked at delta 130ms
        P4IE |= (BIT4); 
        usonic++;
        break;
    case 3:             // Checked at delta 195ms
        P5IE |= (BIT4); 
        usonic = 0;
        break;
    default:
        usonic = 0;
        break;
    }

    while(TA0R < 10);

    // Lower trigger to the ultra sonic
    P4OUT &= ~(BIT3);
}
