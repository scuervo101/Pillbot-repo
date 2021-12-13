/*
 * USonicDriver.h
 *
 */

#ifndef USONICDRIVER_H_
#define USONICDRIVER_H_

#include <msp430.h>
#include <stdint.h>

/*
 *  us0_range (float) Final cm value for ultrasonic 0
 *  us1_range (float) Final cm value for ultrasonic 1
 *  us2_range (float) Final cm value for ultrasonic 2
 *  us3_range (float) Final cm value for ultrasonic 3
 */
float us0_range;
float us1_range;
float us2_range;
float us3_range;

/*
 *  usonic_Init
 *
 *  @brief Initialize usonic requirements and pins on port 4.3 and 4.4
 *
 *  @param
 *  @return
 */
void usonic_Init( void );

/*
 *  usonic_Start
 *
 *  @brief Start the ultrasonic sensor routine. Enables the port4 and timer interrupts
 *
 *  @param
 *  @return
 */
void usonic_Start( void );

#endif /* USONICDRIVER_H_ */
