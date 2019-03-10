
#ifndef LCD_H_
#define LCD_H_
#include "Keypad.h"
#include "NokiaLCD.h"

#include "WordOperations.h"
#include "WrongAnswer.h"


/************************
 * A5 - Select
 * A6 - Enter
 * A7 - Reset 
 ************************/

unsigned char welcomeMessage[67] = "Let's play Hangman! Player 1, enter a word for Player 2 to guess.";
unsigned char front;
unsigned char LCDindex = 0;
unsigned char customChar[8] = {0x0E, 0x0A, 0x0E, 0x04, 0x0E, 0x04, 0x04, 0x0A}; //hangman charactr


unsigned char displayGuess[17] = {'_'}; //shows player 2 correct guesses
unsigned char P2Guess; //holds letter player 2 guessed

unsigned char WTG_Index = 0; //world to guess index
unsigned char lastClicked;  //last letter user clicked

unsigned char win = 0;
unsigned char LCDReset = 0;


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

void CheckGuessed(){

	letterFound = 0; //letter exists in word
	unsigned char b = 0;
	while((b < WTG_Index) && !letterFound){
		if(wordToGuess[b] == P2Guess){
			displayGuess[b] = wordToGuess[b];
			letterFound = 1;
		}
		b++;
	}//end while
	
	if(letterFound == 0){
		strike++;
		wrong = 1;
	}
}

void CheckCorrect(){
	unsigned char checkWin = 1;
	win = 0;
	unsigned char b = 0;
	while((b < WTG_Index) && checkWin){
		if(displayGuess[b] == '_'){
			checkWin = 0;
		}
		b++;
	}//end while
	
	if(checkWin){
		win = 1;
	}
}


enum LCD_States{Init, Wait, WelcomeLCD, P1InputLCD, P2InputLCD, WinLCD, LoseLCD, 
	ResetLCD};
unsigned char count = 0; //counts how long display message is being displayed


int LCD_Tick(int state){
	switch (state)
	{
		case Init : state = WelcomeLCD;
					LCD_ClearScreen();
					PORTA = SetBit(PORTA,2,0);
					PORTA = SetBit(PORTA,3,0);
// 					//used to set image on nokia screen
//  					lcd_setXY(0x40,0x80);
//  					N5110_image(&head_body_arm_leg2);
					
					
		break;
		case Wait : if(GetBit(~PINA,7)){
						state = ResetLCD;	
					}else{
						state = Wait;
					}
		break;
		case WelcomeLCD :	if(GetBit(~PINA,7)){
								state = ResetLCD;
							}else if(count <= 51){ 
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
		case P1InputLCD :	if(GetBit(~PINA,7)){
								state = ResetLCD;
							}else if(GetBit(~PINA, 6)){
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
		case P2InputLCD :	if(GetBit(~PINA,7)){
								state = ResetLCD;
								
							}else if(strike == 6){
								state = LoseLCD;
								count = 0;
							} else if(win){
								state = WinLCD;
								count = 0;
							}
							else
							{
								state = P2InputLCD;
							}
		break;
		case WinLCD:	if(GetBit(~PINA,7)){
							state = ResetLCD;
						}else if(count <= 20){
							state = WinLCD;
						}else if(count > 20){
							state = Wait;
							LCD_ClearScreen();
							LCD_DisplayString(1, "Press RESET to start a new game");
						}
						
		break;
		case LoseLCD :	if(GetBit(~PINA,7)){
							state = ResetLCD;
						}else if(count <= 20){
							state = LoseLCD;
						}else if(count > 20)
						{
							state = Wait;
							LCD_ClearScreen();
							LCD_DisplayString(1, "Press RESET to start a new game");
						}
		break;
		case ResetLCD : state = WelcomeLCD;
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
		case P1InputLCD :	if(GetBit(~PINA,5)){
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
 		case P2InputLCD :	if(GetBit(~PINA,5)){
									P2Guess = lastClicked;
									character = ' ';
									click = 0;
									LCD_Cursor(1);
									LCD_WriteData(' ');
									LCD_Cursor(1);
						
									CheckGuessed();
									CheckCorrect();
									for(unsigned char a = 0; a < WTG_Index; a++){
										LCD_Cursor(a + 17);
										LCD_WriteData(displayGuess[a]);
									}
					
							}
							LCD_Cursor(1);
							if(character != ' '){
								lastClicked = character;
								LCD_WriteData(character);
							}
				
							
		break;
		case WinLCD:	if(count % 2 == 0){
							LCD_ClearScreen();
						}else {
							LCD_DisplayString(1, "YOU WIN! YOU WIN! YOU WIN!");
						}
						count++;
		break;
		case LoseLCD :	if(count % 2 == 0){
							LCD_ClearScreen();
						}else {
							LCD_DisplayString(1, "YOU LOSE! YOU LOSE! YOU LOSE!");
						}
						count++;
						
		break;
		case ResetLCD :	state = WelcomeLCD;
						count = 0;
						LCD_ClearScreen();
						LCDindex = 1;
						WTG_Index = 0; //world to guess index
						lastClicked = ' ';  //last letter user clicked
						P2Guess = ' ';
						for(unsigned char k = 0; k < 17; k++){
							displayGuess[k] = '_';
							wordToGuess[k] = ' ';
						}
						letterFound = 0; //checks is the letter P2 guess was in P1s word
						WA_Count = 0;
						index = 1;
						click = 0;
						strike = 0;
						counter = 0;
						win = 0; 
						NOKIAReset = 1;
						WAReset = 1;
						PORTA = SetBit(PORTA,2,0);
						PORTA = SetBit(PORTA,3,0);
		break;
	}//end Initializations
	
	
	return state;
}


#endif /* LCD_H_ */