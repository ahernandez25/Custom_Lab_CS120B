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



unsigned char character = ' ';
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
			case '\0': character = ' '; break ; // All 5 LEDs on
			case '1': character = ' '; break ; // hex equivalent
			case '2': 
						if(click % 3 == 1){
							character = 'A';
						}else if(click % 3 == 2){
							character = 'B';
						}else if(click % 3 == 0){
							character = 'C';
						} 
			break;
			case '3':	if(click % 3 == 1){
							character = 'D';
						}else if(click % 3 == 2){
							character = 'E';
						}else if(click % 3 == 0){
							character = 'F';
						}
			break ;
			case '4':	if(click % 3 == 1){
							character = 'G';
						}else if(click % 3 == 2){
							character = 'H';
						}else if(click % 3 == 0){
							character = 'I';
						}
			break ;
			case '5': if(click % 3 == 1){
						character = 'J';
					  }else if(click % 3 == 2){
						character = 'K';
					  }else if(click % 3 == 0){
						character = 'L';
					  }
			break ;
			case '6': if(click % 3 == 1){
						character = 'M';
					}else if(click % 3 == 2){
						character = 'N';
					}else if(click % 3 == 0){
						character = '0';
					}
			break ;
			case '7':	if(click % 4 == 1){
							character = 'P';
						} else if(click % 4 == 2){
							character = 'Q';
						} else if(click % 4 == 3){
							character = 'R';
						} else if(click % 4 == 0){
							character = 'S';
						}
			break ;
			case '8': if(click % 3 == 1){
						character = 'T';
					} else if(click % 3 == 2){
						character = 'U';
					} else if(click % 3 == 3){
						character = 'V';
					}
			break ;
			case '9': if(click % 4 == 1){
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
		default: State = GetKey;
		break;
	}
	
	return state;
}





#endif /* KEYPAD_H_ */