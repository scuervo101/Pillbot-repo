/*
 * SpeakerDriver.h
 *
 */

#ifndef SPEAKERDRIVER_H_
#define SPEAKERDRIVER_H_

#include <msp430.h>
#include <stdint.h>

/*
 *  speaker_Init
 *
 *  @brief Initialize speaker on pins 4.6, 4.5
 * 
 *  @param
 *  @return
 */
void speaker_Init( void );

/*
 *
 *  play_Speaker
 * 
 *  @brief Plays audio through the speaker
 * 
 *  @param
 *  @return
 */
void play_Speaker( void );

/*
 *
 *  stop_Speaker
 * 
 *  @brief Stops audio through the speaker
 * 
 *  @param
 *  @return
 */
void stop_Speaker( void );


#endif /* SPEAKERDRIVER_H_ */