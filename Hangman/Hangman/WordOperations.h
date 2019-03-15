/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the functions to reset the welcome message and 
 * correctly display the score on the screen. 
 */

#ifndef WORDOPERATIONS_H_
#define WORDOPERATIONS_H_

#include <avr/eeprom.h>

unsigned char wordToGuess[17] = {' '}; //array for word to be guessed
unsigned char ScoreArray[4] = {0};		
unsigned char hundreds;		//hundres place of score
unsigned char thousands;	//thousands place of score
unsigned char tens;			//tens place of score
unsigned char ones;			//ones place of score

void ResetWelcomeMessage(unsigned char message[67]){
	
	unsigned char done = 0;
	unsigned char frontChar;
	
	while(!done){
		if(message[0] == 'L'){
			done = 1;
		}else{
			frontChar = message[0];
			
			for (unsigned char j = 0; j < 67; j++) {
				message[j] = message[j + 1];
			}
			// put the saved character on the end
			message[67] = frontChar;
		}
	}
}

/* saves each place of the score in a different variable to be 
 * correctly displayed on LCD screen */
void ReturnHighScore(unsigned short newScore){
	thousands = newScore / 1000;
	hundreds = (newScore % 1000) / 100;
	 tens = (newScore % 100) / 10;
	ones = newScore % 10;
}

#endif /* WORDOPERATIONS_H_ */