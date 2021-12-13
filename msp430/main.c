#include <msp430.h> 
#include <stdint.h>

#include "RPMCDriver.h"
#include "SystemControl.h"
#include "SpeakerDriver.h"

void scan();

void killSystem();

static void peripheral_Init();

/**
 * main.c
 */
int main(void)
{
	// Initialize System
    system_Init();

	// Initialize system peripherals
	peripheral_Init();

	// Enable Interrupts
	globalInt_Enable();

	// Enter raspi handshake to establish communication
	rp_Handshake();

	// Init control flags
	uint8_t pickup_flag = 0;
	uint8_t paused_flag = 0;
	long int volt_poll_cnt = 0;

	// Voltage variables
	uint16_t mc_adc = 0;
	float voltage = 0.0f;

	// Collision test variable
	uint8_t hits = 0;

	while(1)
	{
		// Poll voltage adc from mc every
		if (volt_poll_cnt == 1000000) {
			// Reset volt counter
			volt_poll_cnt = 0;

			// Send Volt read signal
			mc_SendCmd(MOTORVOLT);

			// Poll until received 1st byte of data
			while (*mc_rx_flag_ptr == 0);
            *mc_rx_flag_ptr = 0;
            mc_adc = *mc_rx_ptr;
            mc_adc <<= 8;

			// Poll until received 2nd byte of data
			while (*mc_rx_flag_ptr == 0);
			*mc_rx_flag_ptr = 0;
			mc_adc |= *mc_rx_ptr;

			// Calculate voltage value
			voltage = (mc_adc * (5.1f)) / 325.632f;

			if (voltage < 11.3f) {
				killSystem();
			}
		}
		else {
			volt_poll_cnt++;
		}

//		// Collision override
		if ((P5IN & BIT5)) {
			// Report collision to rp and take control
			rp_SendCmd(STOPPEDCOL);

			P4OUT |= BIT0;

			// Wait until the raspi acknowledges it
			while(*raspi_rx_ptr != RP_ACKSTOP);
			*raspi_rx_flag_ptr = 0;

			// Execute
			sudo_mc_SendCmd(MOTORBACKWARD);
			delay_ms(1500);
			sudo_mc_SendCmd(MOTORSTOP);

			// // Check if previously in deliver mode
			// if (*raspi_rx_flag_ptr == RP_ACKDLV) {
			// 	*raspi_rx_flag_ptr = 0;
			// 	play_Speaker();
			// 	pickup_flag = 1;
			// }

			// Release control
			rp_SendCmd(RLSCTRL);

			// // Clear collision flag on mc
			// sudo_mc_SendCmd(MOTORCLR);

        //    // Clear collision flag and hold led
        //    *mc_col_flag_ptr = 0;
			P4OUT &= ~BIT0;

			// Prevent queue command to run
			*raspi_rx_flag_ptr = 0;

			hits++;
		}
		// Raspi Command received handler
		else if (*raspi_rx_flag_ptr) {
			// Check if in pause state
			if (paused_flag) {
				if (*raspi_rx_ptr == PAUSE) {
					rp_SendCmd(ACKSTOP);
					paused_flag = 0;
				}
				else {
					rp_SendCmd(PAUSED);
				}
			}
			else {
				switch (*raspi_rx_ptr) {
				// Simple movement Commands
				case MOVESTOP:
					rp_SendCmd(ACKSTOP);
					mc_SendCmd(MOTORSTOP);
					break;

				case MOVEFORWARD:
					rp_SendCmd(ACKFORWARD);
					mc_SendCmd(MOTORFORWARD);
					break;

				case MOVEBACKWARD:
					rp_SendCmd(ACKBACKWARD);
					mc_SendCmd(MOTORBACKWARD);
					break;

				case ROTLEFT:
					rp_SendCmd(ACKLEFT);
					mc_SendCmd(MOTORLEFT);
					break;

				case ROTRIGHT:
					rp_SendCmd(ACKRIGHT);
					mc_SendCmd(MOTORRIGHT);
					break;

				// Spin Command
				case SCAN:
					rp_SendCmd(ACKSCANNING);
					mc_SendCmd(MOTORSTOP);
					scan();
					break;

				// Speaker Command
				case SPEAKERALRT:
					rp_SendCmd(ACKSPEAKER);
					play_Speaker();
					pickup_flag = 1;
					break;

				// Mode Commands
				case PAUSE:
					rp_SendCmd(PAUSED);
					mc_SendCmd(MOTORSTOP);
					paused_flag = 1;
					break;

				case KILL:
					mc_SendCmd(MOTORSTOP);
					killSystem();
					break;

				default:
					break;
				}
			}

			*raspi_rx_flag_ptr = 0;
		}

		// Wait for someone to pick up pill bottle if they found it
		if ((P5IN & BIT6) && (pickup_flag)) {
			stop_Speaker();
			rp_SendCmd(PILLDELIVERED);
			pickup_flag = 0;
		}
	}
}

void scan()
{
	// Play speaker to represent a scan
	//play_Speaker();
	//delay_ms(1000);
	mc_SendCmd(SPEEDUP);
	delay_ms(10);
	mc_SendCmd(MOTORRIGHT);
	delay_ms(10000);
	mc_SendCmd(SLOWDOWN);
	//stop_Speaker();
}

void killSystem()
{
	// If Kill command did not come from the raspi, alert the raspi
	if (*raspi_rx_ptr != KILL)
		rp_SendCmd(FAULT);

	mc_SendCmd(MOTORSTOP);
	stop_Speaker();
	P4OUT &= ~(BIT1); // Turn off green LED

	// System has been killed, waiting for restart
	while(1);
}

static void peripheral_Init()
{
	// Init raspi and MC communication
	rpmc_Init();

	// Init Speaker
	speaker_Init();

	// Init delivery button
	P5DIR &= ~(BIT6 | BIT5);

	// Init led
	P4DIR |= BIT0;
}
