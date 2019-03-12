

#ifndef LOWESTTIME_H_
#define LOWESTTIME_H_

#include "WordOperations.h"

unsigned short incrementTime;

unsigned char startTimer;
unsigned char stopTimer;

unsigned char prevHighScore;

unsigned char SetBit1( unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}


enum LowestTimeStates {LT_Init, LT_Wait, LT_Count, LT_Reset};
	
int LT_Tick(int state){
	
	switch(state){
		case LT_Init :	incrementTime = 0;
						startTimer = 0;
						stopTimer = 0;
						state = LT_Wait;
		break;
		case LT_Wait :	if(startTimer){
							state = LT_Count;
						}else{
							state = LT_Wait;
						}
		break;
		case LT_Count :	if(startTimer && !stopTimer){
							state = LT_Count;
						}else if(startTimer	&& stopTimer) {
							state = LT_Reset;
						} else if(!startTimer && !stopTimer){
							state = LT_Reset;
						}
		break;
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
		case LT_Count :	incrementTime++;			
		break;
		case LT_Reset : if(startTimer && stopTimer){
							prevHighScore = eeprom_read_word((short*)46);
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