/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the functions and task function to run the 4x4 keypad matrix 
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "io.c"
#include "timer.h"

//returns bit from char passed in
unsigned char GetBit( unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

unsigned char character = ' ';	//holds what character is pressed on keypad
unsigned char index = 1;		
unsigned char click = 0;		//counts how many times the button was clicked in order
								// to choose correct letter

unsigned char GetKeypadKey() {
	
	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { click++; return ('1');  }
	if (GetBit(PINC,1)==0) { click++;  return ('4'); }
	if (GetBit(PINC,2)==0) { click++; return ('7'); }
	if (GetBit(PINC,3)==0) { click++; return ('*'); }
	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { click++; return ('2'); }
	if (GetBit(PINC,1)==0) { click++; return ('5'); }
	if (GetBit(PINC,2)==0) { click++; return ('8'); }
	if (GetBit(PINC,3)==0) { click++; return ('0'); }
	
	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { click++; return ('3'); }
	if (GetBit(PINC,1)==0) { click++; return ('6'); }
	if (GetBit(PINC,2)==0) { click++; return ('9'); }
	if (GetBit(PINC,3)==0) { click++; return ('#'); }
	
	// Check keys in col 4
	PORTC = 0x7F; // Enable col 6 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { click++; return ('A'); }
	if (GetBit(PINC,1)==0) { click++; return ('B'); }
	if (GetBit(PINC,2)==0) { click++; return ('C'); }
	if (GetBit(PINC,3)==0) { click++; return ('D'); }
		
	return ('\0'); // default value
}

/* declares states for keypad tick function*/
enum CheckKeypad_States {Wait_KP, GetKey, KP_Display} State;
unsigned char x;	//stores return on getKeypadKey();

/* checks the current state and makes transitions or calls functions based on the 
 * current state */
int KeypadTick(int state){
	switch (state){
		/* sets state to GetKey */
		case Wait_KP : state = GetKey;
		break;
		/* sets state to KP_Display */
		case GetKey : state = KP_Display;
		break;
		/* sets state to GetKey */
		case KP_Display : state = GetKey;
		break;
		default: State = GetKey;
		break;
	}//end transitions
	
	switch (state)
	{
		case Wait_KP :
		break;
		/* gets the key of the button pressed  */
		case GetKey : x = GetKeypadKey();
		break;
		case KP_Display :
		
							switch (x) {
								/* no button pressed */
								case '\0': character = ' '; break ; // All 5 LEDs on
								/* button 1 pressed */
								case '1': character = ' '; break ; // hex equivalent
								/* button 2 pressed */
								case '2': 
											/* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'A';
											}else if(click % 3 == 2){
												character = 'B';
											}else if(click % 3 == 0){
												character = 'C';
											} 
								break;
								/* button 3 pressed */
								case '3':	/* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'D';
											}else if(click % 3 == 2){
												character = 'E';
											}else if(click % 3 == 0){
												character = 'F';
											}
								break ;
								/* button 4 pressed */
								case '4':	/* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'G';
											}else if(click % 3 == 2){
												character = 'H';
											}else if(click % 3 == 0){
												character = 'I';
											}
								break ;
								/* button 5 pressed */
								case '5':  /* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'J';
											}else if(click % 3 == 2){
												character = 'K';
											}else if(click % 3 == 0){
												character = 'L';
											}
								break ;
								/* button 6 pressed */
								case '6':	/* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'M';
											}else if(click % 3 == 2){
												character = 'N';
											}else if(click % 3 == 0){
												character = 'O';
											}
								break ;
								/* button 7 pressed */
								case '7':	/* sets the character depending on the 
											 * value of click */
											if(click % 4 == 1){
												character = 'P';
											} else if(click % 4 == 2){
												character = 'Q';
											} else if(click % 4 == 3){
												character = 'R';
											} else if(click % 4 == 0){
												character = 'S';
											}
								break ;
								/* button 8 pressed */
								case '8':	/* sets the character depending on the 
											 * value of click */
											if(click % 3 == 1){
												character = 'T';
											} else if(click % 3 == 2){
												character = 'U';
											} else if(click % 3 == 3){
												character = 'V';
											}
								break ;
								/* button 9 pressed */
								case '9':	/* sets the character depending on the 
											 * value of click */
											if(click % 4 == 1){
												character = 'W';
											} else if(click % 4 == 2){
												character = 'X';
											} else if(click % 4 == 3){
												character = 'Y';
											} else if(click % 4 == 0){
												character = 'Z';
											}
								break ;
								case 'A':  break ;
								case 'B': break ;
								case 'C': break ;
								case 'D': break ;
								case '*': break ;
								case '0': break ;
								case '#': break ;
								default : break ; // Should never occur. Middle LED
		}
		break;
		default: 
		break;
	}
	
	return state;
}





#endif /* KEYPAD_H_ */