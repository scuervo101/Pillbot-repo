/*
 * I2CDriver.c
 *
 */

#include "I2CDriver.h"

static void i2c_gpio_Init( void );

/*
 *
 *  i2c_Master_Init
 * 
 *  @brief Initialize I2C Master for eUSCI_B0 on port 1
 * 
 *  @param slave_addr (uint16_t) Address for i2c slave
 *  @return
 */
void i2c_Master_Init( uint16_t slave_addr )
{
    // Reset the communication peripheral
    UCB0CTL1 |= UCSWRST;

    // Init pins for port 1 i2c
    i2c_gpio_Init();

    // Set to i2c master mode
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL_3 | UCSYNC;

    // Set to 7 bit addressing
    UCB0CTLW0 &= ~UCSLA10;

    // Set baudrate to SMCLK/4 (SMCLK = 20Mhz)
    UCB0BRW = 0x000c8;

    // Set slave address
    UCB0I2CSA = slave_addr;

    // Set communication peripheral to operation mode
    UCB0CTL1 &= ~UCSWRST;
}

/*
 *
 *  i2c_SendBytes
 * 
 *  @brief Send bytes via i2c
 * 
 *  @param values (uint16_t * const) Array of values
 *  @param length (uint16_t) Size of array
 *  @return
 */
void i2c_SendBytes(uint16_t * const values, uint16_t length)
{
    // Wait for stop flag to be cleared
    while(UCB0CTLW0 & UCTXSTP);

    // Transmitt start flag
    UCB0CTLW0 |= UCTR | UCTXSTT;

    // Wait for start flag to be cleared
    while(UCB0CTLW0 & UCTXSTT);

    // Loop through array and write the byte to the TX buffer
    for (uint16_t i = 0; i < length; i++)
    {
        while(!(UCB0IFG & UCTXIFG0));
        UCB0TXBUF = values[i];
    }

    // Transmitt stop flag
    UCB0CTLW0 |= UCTXSTP;
}

/*
 *
 *  i2c_gpio_Init
 * 
 *  @brief Initialize GPIO pins for eUSCI_B0 i2c functionality on port 1
 *
 *  @param
 *  @return
 */
static void i2c_gpio_Init( void )
{
    // Set as inputs for Pins
    P1DIR &= ~(BIT3 | BIT2);   

    // Set output to high for Pins
    P1OUT |= (BIT3 | BIT2);

    // Enable pull-up for Pins
    P1REN |= (BIT3 | BIT2);

    // Set P1SEL = 0x01 for Pins
    P1SEL1 &= ~(BIT3 | BIT2);
    P1SEL0 |= (BIT3 | BIT2);
}
