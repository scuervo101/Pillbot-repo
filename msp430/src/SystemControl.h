/*
 * SystemControl.h
 *
 */

#ifndef SYSTEMCONTROL_H_
#define SYSTEMCONTROL_H_

#include <msp430.h>
#include <stdint.h>

/*
 *  system_Init
 *
 *  @brief Initialize the msp430 with WD disable, Ports not tristated, and 20MHz clock
 *
 *  @param
 *  @return
 */
void system_Init( void );

/*
 *  globalInt_Enable
 *
 *  @brief Enables global interrupts
 *
 *  @param
 *  @return
 */
void globalInt_Enable( void );

/*
 *  globalInt_Disable
 *
 *  @brief Disables global interrupts
 *
 *  @param
 *  @return
 */
void globalInt_Disable( void );

/*
 *
 *  delay
 * 
 *  @brief Debug function to add a delay of x cycles
 * 
 *  @param dval (long) number of cycles to delay by
 */
inline void delay(long dval);


/*
 *
 *  delay_ms
 * 
 *  @brief Debug function to add a delay of x ms
 * 
 *  @param dval (long) number of ms to delay by
 */
inline void delay_ms(long dval);

#endif /* SYSTEMCONTROL_H_ */
