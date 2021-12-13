/*
 * SystemControl.h
 *
 */

#include "SystemControl.h"

/*
 *  system_Init
 */
void system_Init( void )
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Set clock to 20Mhz
    CSCTL0 = 0;
    CSCTL1 = (0b101 << 1) | (DISMOD) | (DCOFTRIMEN) | (0b111 << 4);

    // Set F clock divide to 1
    CSCTL2 &= ~(0x7 << 12);
    CSCTL2 |= (0x0 << 12);

    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
                                // to activate previously configured port settings

    // Set status LED1 to one
    P4DIR |= BIT1;
    P4OUT |= BIT1;
}

/*
 *  delay
 */
inline void delay(long dval){for(volatile long j = 0; j < dval; j++);}

/*
 *  delay_ms
 */
inline void delay_ms(long dval){for(volatile long j = 0; j < dval; j++){delay(1000);}}

/*
 *  globalInt_Enable
 */
void globalInt_Enable( void )
{
    // enable global interrupt
    __bis_SR_register(GIE);
}

/*
 *  globalInt_Disable
 */
void globalInt_Disable( void )
{
    // disable global interrupt
    __bic_SR_register(GIE);
}
