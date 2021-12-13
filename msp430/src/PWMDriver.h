/*
 * PWMDriver.h
 *
 */

#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include <msp430.h>
#include <stdint.h>

/*
 *  pwm_Init
 *
 *  @brief Initialize PWM on pins 4.6, 4.5
 * 
 *  @param
 *  @return
 */
void pwm_Init( void );

/*
 *  set_Duty
 *
 *  @brief Change the duty cycle of the PWM signal
 * 
 *  @param duty_cycle (uint16_t) Set the duty cycle of the PWM signal
 *  @return
 */
void set_Duty( uint16_t duty_cycle );

#endif /* PWMDRIVER_H_ */
