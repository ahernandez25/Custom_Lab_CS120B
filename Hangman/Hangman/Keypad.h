/*
 * Keypad.h
 *
 * Created: 2/20/2019 8:54:04 PM
 *  Author: ashly
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "io.c"
#include "timer.h"

unsigned char GetBit( unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



unsigned char character = 0x1F;
unsigned char index = 1;
unsigned char click = 0;


unsigned char GetKeypadKey() {
	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return ('1'); }
	if (GetBit(PINC,1)==0) { return ('4'); }
	if (GetBit(PINC,2)==0) { return ('7'); }
	if (GetBit(PINC,3)==0) { return ('*'); }
	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return ('2'); }
	if (GetBit(PINC,1)==0) { return ('5'); }
	if (GetBit(PINC,2)==0) { return ('8'); }
	if (GetBit(PINC,3)==0) { return ('0'); }
	
	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return ('3'); }
	if (GetBit(PINC,1)==0) { return ('6'); }
	if (GetBit(PINC,2)==0) { return ('9'); }
	if (GetBit(PINC,3)==0) { return ('#'); }
	
	// Check keys in col 4
	PORTC = 0x7F; // Enable col 6 with 0, disable others with 1’s
	asm ( "nop" ); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return ('A'); }
	if (GetBit(PINC,1)==0) { return ('B'); }
	if (GetBit(PINC,2)==0) { return ('C'); }
	if (GetBit(PINC,3)==0) { return ('D'); }
		
	click++;	
	
	return ('\0'); // default value
}

enum CheckKeypad_States {Wait_KP, GetKey, KP_Display} State;
unsigned char x;

int KeypadTick(int state){
	switch (state){
		case Wait_KP : state = GetKey;
		break;
		case GetKey : state = KP_Display;
		break;
		case KP_Display : state = GetKey;
		break;
		default: State = GetKey;
		break;
	}//end transitions
	
	switch (state)
	{
		case Wait_KP :
		break;
		case GetKey : x = GetKeypadKey();
		break;
		case KP_Display :
		
		switch (x) {
			case '\0': character = 0x1F; break ; // All 5 LEDs on
			case '1': character = 0x01F; break ; // hex equivalent
			case '2': character = 0x02;
			/*if(click % 3 == 1){
						character = 'A';
					  }else if(click % 3 == 2){
						  character = 'B';
					  }else if(click % 3 == 0){
						  character = 'C';
					  } 
				break ; */
			break;
			case '3': character = 0x03; break ;
			case '4': character = 0x04; break ;
			case '5': character = 0x05; break ;
			case '6': character = 0x06; break ;
			case '7': character = 0x07; break ;
			case '8': character = 0x08; break ;
			case '9': character = 0x09; break ;
			case 'A': character = 's'; break ;
			case 'B': character = 'e'; break ;
			case 'C': character = 'C'; break ;
			case 'D': character = 'D'; break ;
			case '*': character = '*'; break ;
			case '0': character = 0; break ;
			case '#': character = '#'; break ;
			default : character = 0x1B; break ; // Should never occur. Middle LED
		}
		break;
		default: State = GetKey;
		break;
	}
	
	return state;
}





#endif /* KEYPAD_H_ */