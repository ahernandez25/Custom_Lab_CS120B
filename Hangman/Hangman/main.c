/*
 * Hangman.c
 *
 * Created: 2/19/2019 3:06:42 PM
 * Author : ashly
 */ 

#include <avr/io.h>
#include "LCD.h"

typedef struct task {
	/*Tasks should have members that include: state, period,
	a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)( int ); //Task tick function
} task;

	static task Tasks[2];

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRA = 0xFF; PORTA = 0xFF; // LCD control lines
		DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
		DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	unsigned long LCDPeriod = 500;
	unsigned long KPPeriod = 500;
	unsigned long period = 100;
	unsigned char i = 0;
	
	Tasks[i].state = Init;
	Tasks[i].period = LCDPeriod;
	Tasks[i].elapsedTime = LCDPeriod;
	Tasks[i].TickFct = &LCD_Tick;
	i++;
	Tasks[i].state = Wait_KP;
	Tasks[i].period = KPPeriod;
	Tasks[i].elapsedTime = KPPeriod;
	Tasks[i].TickFct = &KeypadTick;
	
	//LCDBuildChar(0, customChar);
	TimerSet(period);
	TimerOn();
	LCD_init();
	
	
	//LCD_DisplayString(1,"Please Work");
	
	while (1) {
		for ( i = 0; i < 2; i++ ) {
			// Task is ready to tick
			if ( Tasks[i].elapsedTime == Tasks[i].period ) {
				// Setting next state for task
				Tasks[i].state = Tasks[i].TickFct(Tasks[i].state);
				// Reset the elapsed time for next tick.
				Tasks[i].elapsedTime = 0;
			}
			Tasks[i].elapsedTime += period;
		}
		
		while (!TimerFlag){}
		TimerFlag = 0;
		
	}
}

