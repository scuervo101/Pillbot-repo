/*
 * RPMCDriver.h
 *
 */

#ifndef RPMCDRIVER_H_
#define RPMCDRIVER_H_

#include <msp430.h>
#include <stdint.h>

#include "UARTDriver.h"
#include "SystemControl.h"

/*
 *  raspi_rx_ptr (uint16_t*) Pointer to the raspi's rx data buffer
 */
uint16_t* raspi_rx_ptr;

/*
 *  raspi_rx_flag_ptr (uint8_t*) Pointer to flag that is true when raspi has sent new data
 */
uint8_t* raspi_rx_flag_ptr;

/*
 *  mc_rx_ptr (uint16_t*) Pointer to the mc's rx data buffer
 */
uint16_t* mc_rx_ptr;

/*
 *  mc_rx_flag_ptr (uint8_t*) Pointer to flag that is true when mc has sent new data
 */
uint8_t* mc_rx_flag_ptr;

/*
 *  mc_col_flag_ptr (uint8_t*) Pointer to collosion flag for the motor controller
 */
uint8_t* mc_col_flag_ptr;


/*
 *  Defines for Motor Controller
 */
#define MOTORFORWARD    'b'
#define MOTORBACKWARD   'f'
#define MOTORSTOP       's'
#define MOTORLEFT       'l'
#define MOTORRIGHT      'r'
#define MOTORVOLT       'V'
#define MOTORCLR        'c'
#define SPEEDUP         0x69
#define SLOWDOWN        0x64

/*
 *  Defines for Raspi msgs
 */
// Communication msgs
#define HANDSHAKE       0xff
// Acknowledge msgs
#define ACKSTOP         0x10
#define ACKFORWARD      0x11
#define ACKBACKWARD     0x12
#define ACKLEFT         0x13
#define ACKRIGHT        0x14
#define ACKSCANNING     0x15
#define ACKSPEAKER      0x16
// Manual ctrl cmds
#define STOPPEDUSC      0x20
#define STOPPEDCOL      0x21
#define RLSCTRL         0x22
// Delivery flag
#define PILLDELIVERED   0x30
// Control msgs
#define FAULT           0x07
#define ERRORACK        0x03
#define PAUSED           0x01

/*
 *  Defines for Raspi rx cmds
 */
// Communication msgs
//#define HANDSHAKE       0xff
// Control cmds
#define MOVESTOP        0x10
#define MOVEFORWARD     0x11
#define MOVEBACKWARD    0x12
#define ROTLEFT         0x13
#define ROTRIGHT        0x14
#define SCAN            0x15
#define SPEAKERALRT     0x16
#define PAUSE           0x0f
#define KILL            0x1f
// Ack cmds
#define RP_ACKSTOP      0x20
#define RP_ACKDLV       0x22
// Fault flag
#define FAULTCON        0x3f

/*
 *  rpmc_Init
 *
 *  @brief Initialize Raspi and MC communication using uart
 * 
 *  @param
 *  @return
 */
void rpmc_Init( void );


/*
 *  rp_Handshake
 *
 *  @brief Handshake for the raspi to establish connection
 * 
 *  @param
 *  @return
 */
void rp_Handshake( void );

/*
 *  rp_SendCmd
 *
 *  @brief Function to send a single command to the raspi
 * 
 *  @param val (uint8_t) byte to send
 *  @return
 */
void rp_SendCmd( uint8_t val );

/*
 *  mc_SendCmd
 *
 *  @brief Function to send a single command to the mc
 * 
 *  @param val (uint8_t) byte to send
 *  @return
 */
void mc_SendCmd( uint8_t val );

/*
 *  mc_SendCmd
 *
 *  @brief Function to send a single command with sudo priv (override mc collision flag) to the mc
 *
 *  @param val (uint8_t) byte to send
 *  @return
 */
void sudo_mc_SendCmd( uint8_t val );

#endif /* SPEAKERDRIVER_H_ */
