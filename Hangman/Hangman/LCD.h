/*
 * LCD.h
 *
 * Created: 2/19/2019 3:11:13 PM
 *  Author: ashly
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "Keypad.h"

/************************
 * A5 - Select
 * A6 - Enter
 * A7 - Reset 
 ************************/

unsigned char welcomeMessage[67] = "Let's play Hangman! Player 1, enter a word for Player 2 to guess.";
unsigned char front;
unsigned char LCDindex = 0;
unsigned char customChar[8] = {0x0E, 0x0A, 0x0E, 0x04, 0x0E, 0x04, 0x04, 0x0A};
unsigned char cursor = 0;
unsigned char clear = 1;
unsigned char wordToGuess[17] = {' '};
unsigned char WTG_Index = 0;
unsigned char lastClicked;

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


enum LCD_States{Init, Wait, WelcomeLCD, P1InputLCD, P2InputLCD, WinLCD, LoseLCD};
unsigned char count = 0; //counts how long display message is being displayed


int LCD_Tick(int state){
	switch (state)
	{
		case Init : state = WelcomeLCD;
					LCD_ClearScreen();
					
					
					
		break;
		case Wait : 
		break;
		case WelcomeLCD : if(count <= 51){ state = WelcomeLCD;}
						  else if(count > 51){state = P1InputLCD;
						  LCD_ClearScreen(); 
						  LCDindex = 1;}
		break;
		case P1InputLCD : state = P1InputLCD;
						if(GetBit(~PINA, 6)){state = P2InputLCD;}
								//LCD_ClearScreen();}
								else{
							  state = P1InputLCD;}
		break;
		case P2InputLCD :
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
				}
				
			}
			LCD_Cursor(LCDindex);
			if(character != ' '){
				lastClicked = character;
				LCD_WriteData(character);
			}
			
				

		
		break;
		case P2InputLCD : LCD_ClearScreen();
		for(unsigned char l = 0; l < WTG_Index; l++){
			LCD_Cursor(l + 1);
			LCD_WriteData(wordToGuess[l]);
		}
		LCD_Cursor(17);
		LCD_WriteData('*');
							
		break;
		case WinLCD:
		break;
		case LoseLCD :
		break;
	}//end Initializations
	
	
	return state;
}


#endif /* LCD_H_ */