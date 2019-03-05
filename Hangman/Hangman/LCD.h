/*
 * LCD.h
 *
 * Created: 2/19/2019 3:11:13 PM
 *  Author: ashly
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "Keypad.h"
#include "NokiaLCD.h"
#include "LCDImages.h"
#include "WordOperations.h"

/************************
 * A5 - Select
 * A6 - Enter
 * A7 - Reset 
 ************************/

unsigned char welcomeMessage[67] = "Let's play Hangman! Player 1, enter a word for Player 2 to guess.";
unsigned char front;
unsigned char LCDindex = 0;
unsigned char customChar[8] = {0x0E, 0x0A, 0x0E, 0x04, 0x0E, 0x04, 0x04, 0x0A}; //hangman charactr


unsigned char displayGuess[17] = {' '}; //shows player 2 correct guesses
unsigned char P2Guess; //holds letter player 2 guessed

unsigned char WTG_Index = 0; //world to guess index
unsigned char lastClicked;  //last letter user clicked
unsigned char letterFound = 0; //checks is the letter P2 guess was in P1s word

void LCDBuildChar(unsigned char loc, unsigned char *p)
{
	unsigned char i;
	if(loc<8) //If valid address
	{
		LCD_WriteCommand(0x40+(loc*8)); //Write to CGRAM
		for(i=0;i<8;i++)
		LCD_WriteData(p[i]); //Write the character pattern to CGRAM
	}
}

unsigned char CheckGuessed(){
	for(unsigned char b = 0; b < 17; b++){
		if(wordToGuess[b] == P2Guess){
			displayGuess[b] = wordToGuess[b];
		}
	}
}


enum LCD_States{Init, Wait, WelcomeLCD, P1InputLCD, P2InputLCD, WinLCD, LoseLCD};
unsigned char count = 0; //counts how long display message is being displayed


int LCD_Tick(int state){
	switch (state)
	{
		case Init : state = WelcomeLCD;
					LCD_ClearScreen();
					SPI_Init();
					N5110_init();
					N5110_clear();
					
					//used to set image on nokia screen
// 					lcd_setXY(0x40,0x80);
// 					N5110_image(&head_body_arm_leg2);
					
					
		break;
		case Wait : 
		break;
		case WelcomeLCD : if(count <= 51){ 
							  state = WelcomeLCD;
						  }
						  else if(count > 51){
							  state = P1InputLCD;
							  LCD_ClearScreen();
							  LCD_DisplayString(17,"16 Letters MAX");
							  LCD_Cursor(1);
							  LCDindex = 1;
						 }
		break;
		case P1InputLCD :	if(GetBit(~PINA, 6)){
								LCD_ClearScreen();
								state = P2InputLCD;
								delay_ms(2);
							
							
							for(unsigned char a = 0; a < WTG_Index; a++){
								displayGuess[a] = '_';
								LCD_Cursor(a + 17);
								LCD_WriteData('_');
							}
								
							LCD_Cursor(1);
							LCDindex = 1;					
						}else{						
							state = P1InputLCD;
						}
		break;
		case P2InputLCD :	if(GetBit(~PINA, 6)){
								
							}
							else
							{
								state = P2InputLCD;
							}
		break;
		case WinLCD:
		break;
		case LoseLCD :
		break;
	}//end Transitions
	
	switch (state)
	{
		case Init : 
		break;
		case Wait :
		break;
		case WelcomeLCD : 		/* LCD_DisplayString(1,welcomeMessage);
								front = welcomeMessage[0];
		
								for (unsigned char j = 0; j < 67; j++) {
									welcomeMessage[j] = welcomeMessage[j + 1];
								}
								// put the saved character on the end
								welcomeMessage[67] = front; */
		
						
			for(LCDindex = 0; LCDindex < 16; LCDindex++){
				LCD_Cursor(LCDindex + 1);
				LCD_WriteData(welcomeMessage[LCDindex]);
			}
		
		
		
		front = welcomeMessage[0];
		
		for (unsigned char j = 0; j < 67; j++) {
			welcomeMessage[j] = welcomeMessage[j + 1];
		}
		// put the saved character on the end
		welcomeMessage[67] = front;
		
		LCDBuildChar(0, customChar);
			LCD_Cursor(17);
			LCD_WriteData(0x00);
			LCD_Cursor(20);
			LCD_WriteData(0x00);
			LCD_Cursor(23);
			LCD_WriteData(0x00);
			LCD_Cursor(26);
			LCD_WriteData(0x00);
			LCD_Cursor(29);
			LCD_WriteData(0x00);
			LCD_Cursor(32);
			LCD_WriteData(0x00);
			LCD_Cursor(35);
			LCD_WriteData(0x00);
		
		count++;
		
			
		break;
		case P1InputLCD :
			
			if(GetBit(~PINA,5)){
				if(WTG_Index < 16){
					LCDindex++;
					wordToGuess[WTG_Index] = lastClicked;
					WTG_Index++;
					character = ' ';
					click = 0;
					
				}
				
			}
			LCD_Cursor(LCDindex);
			if(character != ' '){
				lastClicked = character;
				LCD_WriteData(character);
			}
			
				

		
		break;
 		case P2InputLCD : 
// 		for(unsigned char l = 0; l < WTG_Index; l++){
// 			LCD_Cursor(l + 1);
// 			LCD_WriteData(wordToGuess[l]);
// 		}
// 		LCD_Cursor(17);
// 		LCD_WriteData('*');
// 		SPI_Init();
// 		N5110_init();
// 				N5110_clear();
// 				lcd_setXY(0x40,0x80);
// 				N5110_Data("ElectronicWings");

				if(GetBit(~PINA,5)){
						P2Guess = lastClicked;
						character = ' ';
						click = 0;
					
				}
				LCD_Cursor(1);
				if(character != ' '){
					lastClicked = character;
					LCD_WriteData(character);
				}
				
							
		break;
		case WinLCD:
		break;
		case LoseLCD :
		break;
	}//end Initializations
	
	
	return state;
}


#endif /* LCD_H_ */