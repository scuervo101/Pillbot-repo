/*
 * RPMCDriver.c
 *
 */

#include "RPMCDriver.h"

/*
 *  rpmc_Init
 */
void rpmc_Init( void )
{
	// Init UART
	uartA0_Init(EUART_PORT5);	// Motor Controller uart
	uartA1_Init();				// Raspi uart

    // Set the raspi pointers to reference A1 rx vars
    raspi_rx_ptr = &UARTA1_RxBuf;
    raspi_rx_flag_ptr = &UARTA1_RxBuf_flag;

    // Set the mc pointers to reference A0 rx vars
    mc_rx_ptr = &UARTA0_RxBuf;
    mc_rx_flag_ptr = &UARTA0_RxBuf_flag;
    mc_col_flag_ptr = &UARTA0_alert_flag;
}

/*
 *  rp_Handshake
 */
void rp_Handshake( void )
{
	// Handshake to establish connection

    P4DIR |= BIT2;
    P4OUT |= BIT2;	// Turn on blue status LED

	while (UARTA1_RxBuf != HANDSHAKE);
    UARTA1_RxBuf_flag = 0;

    P4OUT &= ~(BIT2);	// Turn off blue status LED once connection is made

    delay(1000000);

	uartA1_SendByte(HANDSHAKE); // Send the handshake command
}

/*
 *  rp_SendCmd
 */
void rp_SendCmd( uint8_t val )
{
    uartA1_SendByte(val);
}

/*
 *  mc_SendCmd
 */
void mc_SendCmd( uint8_t val )
{
    //if (*mc_col_flag_ptr == 0)
    uartA0_SendByte(val);
}

/*
 *  sudo_mc_SendCmd
 */
void sudo_mc_SendCmd( uint8_t val )
{
    uartA0_SendByte(val);
}
