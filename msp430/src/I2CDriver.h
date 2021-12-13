/*
 * I2CDriver.h
 *
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <msp430.h>
#include <stdint.h>

/*
 *
 *  i2c_Master_Init
 * 
 *  @brief Initialize I2C Master for eUSCI_B0 on port 1
 * 
 *  @param slave_addr (uint16_t) Address for i2c slave
 *  @return
 */
void i2c_Master_Init( uint16_t slave_addr );

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
void i2c_SendBytes(uint16_t * const values, uint16_t length);

#endif /* I2CDRIVER_H_ */
