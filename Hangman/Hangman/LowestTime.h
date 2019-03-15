/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the functions and variables to run the lowsest time task
 */

#ifndef LOWESTTIME_H_
#define LOWESTTIME_H_

#include "WordOperations.h"

unsigned short incrementTime;	//holds how many ticks have passed
unsigned char startTimer;		//starts counting the ticks
unsigned char stopTimer;		//stops counting the ticks
unsigned char prevHighScore;	//checks previous high score

/* declares states for lowest time tick function*/
enum LowestTimeStates {LT_Init, LT_Wait, LT_Count, LT_Reset};
	
int LT_Tick(int state){
	
	switch(state){
		/* initializes tick function */
		case LT_Init :	incrementTime = 0;
						startTimer = 0;
						stopTimer = 0;
						state = LT_Wait;
		break;
		/* if the timer starts, the state is set to lt_count. Else stays in wait.*/
		case LT_Wait :	if(startTimer){
							state = LT_Count;
						}else{
							state = LT_Wait;
						}
		break;
		case LT_Count :	/* if startTimer flag in set, but stopTimer flag is not, state stays
						in lt_count */
						if(startTimer && !stopTimer){
							state = LT_Count;
						}
						/* if startTimer and stopTimer flags are set, state is set to reset */
						else if(startTimer	&& stopTimer) {
							state = LT_Reset;
						} 
						/* if startTimer flag not set, but stopTimer flag is, state is set
						 * to lt_reset */
						else if(!startTimer && !stopTimer){
							state = LT_Reset;
						}
		break;
		/* state is set to lt_wait*/
		case LT_Reset : state = LT_Wait;
		break;
		default :
		break;
	}
	
	switch(state){
		case LT_Init :
		break;
		case LT_Wait :
		break;
		/* increments time */
		case LT_Count :	incrementTime++;			
		break;
		case LT_Reset : //if both flags set, gets prev high score from eeprom
						if(startTimer && stopTimer){
							prevHighScore = eeprom_read_word((short*)46);
							
							//saves new high score if current time is less than prev
							if(prevHighScore > incrementTime){
								eeprom_update_word((short*)46,(incrementTime / 2));
							}	
						}
							
						incrementTime = 0;
						startTimer = 0;
						stopTimer = 0;						
		break;
		default :
		break;
	}
	
	return state;
}



#endif /* LOWESTTIME_H_ */