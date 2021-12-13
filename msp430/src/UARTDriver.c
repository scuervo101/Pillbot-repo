/*
 * UARTDriver.c
 *
 */

#include "UARTDriver.h"

/*
 *  uartA0_gpio_Init
 * 
 *  @brief Initialize GPIO pins for eUSCI_A0 uart functionality on selected port
 *
 *  @param port (uint8_t) Value to select which port to init uart on
 *  @return
 */
static void uartA0_gpio_Init( uint8_t port );

/*
 *  uartA1_gpio_Init
 * 
 *  @brief Initialize GPIO pins for eUSCI_A1 uart functionality on pins 2.5,2.6
 *
 *  @param
 *  @return
 */
static void uartA1_gpio_Init( void );

/*
 *  uartA0_Init
 */
void uartA0_Init( uint8_t port )
{
    // Reset the communication peripheral
    UCA0CTL1 |= UCSWRST;

    // Maintain default UART settings (no parity, lsb, 8-bit, one stop)
    // Set uart clock to SMCLK (12Mhz)
    UCA0CTLW0 |= UCSSEL_3;

    // Set 38400 baud rate with our 20Mhz clock. Recommended settings from page 590 of manual
    UCA0BRW = 32;
    UCA0MCTLW = (UCOS16) | (8 << 4) | (0xEE << 8);

    // Init pins on select port for uart
    uartA0_gpio_Init(port);

    // Release peripheral from reset state
    UCA0CTL1 &= ~UCSWRST;

    // Enable Receive register
    UCA0IE |= UCRXIE;

    // Clear the Interrupt flag
    UCA0IFG &= ~(UCRXIFG);
}

/*
 *  uartA0_SendByte
 */
void uartA0_SendByte(uint8_t val)
{
    // Loop through array and write the byte to the TX buffer
    while(!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = val;
}

/*
 *  uartA0_SendBytes
 */
void uartA0_SendBytes(uint16_t * const values, uint16_t length)
{
    // Loop through array and write the byte to the TX buffer
    for (uint16_t i = 0; i < length; i++)
    {
        while(!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = values[i];
    }
}

/*
 *  uartA0_SendStrings
 */
void uartA0_SendStrings(char * string)
{
    // Loop through array and write the byte to the TX buffer
    while(*string != 0)
    {
        while(!(UCA0IFG & UCTXIFG0));
        UCA0TXBUF = string[0];

        string++;
    }
}

/*
 *  uartA0_gpio_Init
 */
static void uartA0_gpio_Init( uint8_t port )
{
    if (port == EUART_PORT1) {
        // Set as inputs for rx pin
        P1DIR &= ~(BIT5);

        // Set as output for tx pin
        P1DIR |= (BIT4);

        // Set P1SEL = 0x01 for Pins
        P1SEL1 &= ~(BIT5 | BIT4);
        P1SEL0 |= (BIT5 | BIT4);

        // Keep UART pins on port 1
        SYSCFG3 &= ~(USCIA0RMP);

    } else if (port == EUART_PORT5) {
        // Set as inputs for rx pin
        P5DIR &= ~(BIT1);

        // Set as output for tx pin
        P5DIR |= (BIT2);

        // Set P5SEL = 0x01 for Pins
        P5SEL1 &= ~(BIT1 | BIT2);
        P5SEL0 |= (BIT1 | BIT2);

        // Shift UART pins to port 5
        SYSCFG3 |= USCIA0RMP;
    }
}

/* ------------------ A1 Instatiations ------------------ */

/*
 *  uartA1_Init
 */
void uartA1_Init( void )
{
    // Reset the communication peripheral
    UCA1CTL1 |= UCSWRST;

    // Maintain default UART settings (no parity, lsb, 8-bit, one stop)
    // Set uart clock to SMCLK (12Mhz)
    UCA1CTLW0 |= UCSSEL_3;

    // Set 115200 baud rate with our 20Mhz clock. Recommended settings from page 590 of manual
    UCA1BRW = 10;
    UCA1MCTLW = (UCOS16) | (13 << 4) | (0xAD << 8);

    // Init pins on select port for uart
    uartA1_gpio_Init();

    // Release peripheral from reset state
    UCA1CTL1 &= ~UCSWRST;

    // Enable Receive register
    UCA1IE |= UCRXIE;

    // Clear the Interrupt flag
    UCA1IFG &= ~(UCRXIFG);
}

/*
 *  uartA1_SendByte
 */
void uartA1_SendByte(uint8_t val)
{
    // Loop through array and write the byte to the TX buffer
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = val;
}

/*
 *  uartA1_SendBytes
 */
void uartA1_SendBytes(uint16_t * const values, uint16_t length)
{
    // Loop through array and write the byte to the TX buffer
    for (uint16_t i = 0; i < length; i++)
    {
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = values[i];
    }
}

/*
 *  uartA1_SendStrings
 */
void uartA1_SendStrings(char * string)
{
    // Loop through array and write the byte to the TX buffer
    while(*string != 0)
    {
        while(!(UCA1IFG & UCTXIFG0));
        UCA1TXBUF = string[0];

        string++;
    }
}

/*
 *  uartA1_gpio_Init
 */
static void uartA1_gpio_Init( void )
{
    // Set as inputs for rx pin
    P2DIR &= ~(BIT5);

    // Set as output for tx pin
    P2DIR |= (BIT6);

    // Set P2SEL = 0x01 for Pins
    P2SEL1 &= ~(BIT5 | BIT6);
    P2SEL0 |= (BIT5 | BIT6);
}

// _________ Interrupt Service Routines _________
#pragma vector=EUSCI_A0_VECTOR          // Motor Controller
__interrupt void UARTA0_Rx(void)
{
    UARTA0_RxBuf = UCA0RXBUF;
    UARTA0_RxBuf_flag = 1;

    if (UARTA0_RxBuf == 'c') {
        UARTA0_alert_flag = 1;
        // P4DIR |= BIT0;
        // P4OUT |= BIT0;
    }
}

#pragma vector=EUSCI_A1_VECTOR          // Raspi
__interrupt void UARTA1_Rx(void)
{
    UARTA1_RxBuf = UCA1RXBUF;
    UARTA1_RxBuf_flag = 1;

    if (UARTA1_RxBuf == 0x10 || UARTA1_RxBuf == 0x0f || UARTA1_RxBuf == 0x1f) {
        uartA0_SendByte('s');
    }
}

