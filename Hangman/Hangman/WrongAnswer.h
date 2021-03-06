/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the functions and task function to check for a wrong answer
 */

#ifndef WRONGANSWER_H_
#define WRONGANSWER_H_

unsigned char letterFound = 0;	//checks is the letter P2 guess was in P1s word
unsigned char WA_Count = 0;		//checks how many wrong answers were guessed
unsigned char WAReset = 0;		//checks if reset was clicked
unsigned char wrong = 0;		//checks for a wrong answer

unsigned char SetBit( unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

/* declares states for wrong answer task*/
enum WA_States {WA_Wait, SoundBuzzer, Wait_Low, WA_Reset};

int WA_Tick(int state){
	switch(state){
		/*checks for reset. if not, checks if answer is wrong and states to sound buzzer and 
		 * lights up light if true. else stays in wait */
		case WA_Wait :	if(WAReset){
							state = WA_Reset;
						} else if(wrong){
							PORTA = SetBit(PORTA,2,1);
							state = SoundBuzzer;
						}
						else{
							state = WA_Wait;
						}
						
		break;
		/* checks for reset. if not, stays in sound buzzer for 30 ticks. then sets state to
		 * wait low */
		case SoundBuzzer :	if(WAReset){
								state = WA_Reset;
							} else if(WA_Count <= 30){
								state = SoundBuzzer;
							}
							else if(WA_Count > 30){
								state = Wait_Low;
								WA_Count = 0;
								wrong = 0;
							}
							
		break;
		/*checks for reset. if not, checks if the letter guessed was correct and sets state 
		 * to wait low if true. Else it sets to wa_wait. */
		case Wait_Low : if(WAReset){
							state = WA_Reset;
						} else if(letterFound){
							state = Wait_Low;
						}
						else{
							state = WA_Wait;
						}
		break;
		/* sets state to wa_ait*/
		case WA_Reset : state = WA_Wait;
		default: 
		break;
	}
	
	switch(state){
		case WA_Wait :	WA_Count = 0;
						set_PWM(0);
		break;
		case SoundBuzzer :	//increments count and lights up led 
							set_PWM(523.25);
							WA_Count++;
							PORTA = SetBit(PORTA,2,1);
		break;
		case Wait_Low : //resets led
						set_PWM(0);
						PORTA = SetBit(PORTA,2,0);
		break;
		case WA_Reset : //reinitializes all variables
						letterFound = 0; //checks is the letter P2 guess was in P1s word
						WA_Count = 0;
						WAReset = 0;
						wrong = 0;
		break;
		default:
		break;
	}
	
	return state;
};


#endif /* WRONGANSWER_H_ */