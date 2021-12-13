/*
 * UARTDriver.h
 *
 */

#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_

#include <msp430.h>
#include <stdint.h>

/*
 *  UARTA0_RxBuf (uint16_t) Rx buffer data
 */
uint16_t UARTA0_RxBuf;

/*
 *  UARTA0_RxBuf_flag (uint8_t) Flag that is true when UARTA0_RxBuf has new data
 */
uint8_t UARTA0_RxBuf_flag;

/*
 *  UARTA0_alert_flag (uint8_t) Flag that is true when collosion message is sent
 */
uint8_t UARTA0_alert_flag;


/*
 *  UARTA1_RxBuf (uint16_t) Rx buffer data
 */
uint16_t UARTA1_RxBuf;

/*
 *  UARTA1_RxBuf_flag (uint8_t) Flag that is true when UARTA1_RxBuf has new data
 */
uint8_t UARTA1_RxBuf_flag;

/*
 *
 *  Port defines to pass into uartA0_Init to configure the correct ports
 * 
 */
#define EUART_PORT1     0
#define EUART_PORT5     1

/*
 *  uartA0_Init
 *
 *  @brief Initialize UARTA0 on select port
 * 
 *  @params port (uint8_t) Value to select which port to init uart on
 *  @return
 */
void uartA0_Init( uint8_t port );

/*
 *  uartA0_SendByte
 *
 *  @brief Sends a single byte
 * 
 *  @params val (uint8_t) byte to send
 *  @return
 */
void uartA0_SendByte(uint8_t val);

/*
 *  uartA0_SendBytes
 *
 *  @brief Send bytes via uartA0
 * 
 *  @param values (uint16_t * const) Array of values
 *  @param length (uint16_t) Size of array
 *  @return
 */
void uartA0_SendBytes(uint16_t * const values, uint16_t length);

/*
 *  uartA0_SendStrings
 *
 *  @brief Send string via uartA0
 *
 *  @param string (uint16_t * const) String array with delimeter of 0
 *  @return
 */
void uartA0_SendStrings(char * string);

/*
 *  uartA1_Init
 *
 *  @brief Initialize UARTA1 on select port
 * 
 *  @params
 *  @return
 */
void uartA1_Init( );

/*
 *  uartA1_SendByte
 *
 *  @brief Sends a single byte
 * 
 *  @params
 *  @return
 */
void uartA1_SendByte(uint8_t val);

/*
 *  uartA1_SendBytes
 *
 *  @brief Send bytes via uartA1
 * 
 *  @param values (uint16_t * const) Array of values
 *  @param length (uint16_t) Size of array
 *  @return
 */
void uartA1_SendBytes(uint16_t * const values, uint16_t length);

/*
 *  uartA1_SendStrings
 *
 *  @brief Send string via uartA1
 *
 *  @param string (uint16_t * const) String array with delimeter of 0
 *  @return
 */
void uartA1_SendStrings(char * string);

#endif /* UARTDRIVER_H_ */
