/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the main function that runs the while loop which checks all 
 *  tasks. 
 */
 

#include <avr/io.h>
#include "LCD.h"


typedef struct task {
	signed char state;				//Task's current state
	unsigned long int period;		//Task period
	unsigned long int elapsedTime;	//Time elapsed since last task tick
	int (*TickFct)( int );			//Task tick function
} task;

static task Tasks[5];	//Struct off all tasks

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRA = 0xFF; PORTA = 0xFF; // LCD control lines
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	unsigned long LCDPeriod = 500;		//Period for LCD screen task
	unsigned long KPPeriod = 300;		//Period for Keypad input task
	unsigned long WAPeriod = 100;		//Period for Wrong answer task
	unsigned long NokiaPeriod = 500;	//Period for Nokia screen task
	unsigned long LTPeriod = 500;		//Period for Lowest time task
	
	unsigned long period = 100;		//Period for system
	unsigned char i = 0;			//index to initialize task stucks
	
	//Initializes LCD screen task
	Tasks[i].state = Init;
	Tasks[i].period = LCDPeriod;
	Tasks[i].elapsedTime = LCDPeriod;
	Tasks[i].TickFct = &LCD_Tick;
	i++;
	
	//Initializes Keypad task
	Tasks[i].state = Wait_KP;
	Tasks[i].period = KPPeriod;
	Tasks[i].elapsedTime = KPPeriod;
	Tasks[i].TickFct = &KeypadTick;
	i++;
	
	//Initializes wrong answer task
	Tasks[i].state = WA_Wait;
	Tasks[i].period = WAPeriod;
	Tasks[i].elapsedTime = WAPeriod;
	Tasks[i].TickFct = &WA_Tick;
	i++;
	
	//Initializes nokia screen task
	Tasks[i].state = Nokia_Init;
	Tasks[i].period = NokiaPeriod;
	Tasks[i].elapsedTime = NokiaPeriod;
	Tasks[i].TickFct = &Nokia_Tick;
	i++;
	
	//Initializes lowest time task
	Tasks[i].state = LT_Init;
	Tasks[i].period = LTPeriod;
	Tasks[i].elapsedTime = LTPeriod;
	Tasks[i].TickFct = &LT_Tick;
	

	TimerSet(period);	//sets timer to designated period
	TimerOn();			//turns on timer
	LCD_init();			//initializes lcd screen
	PWM_on();			//initializes buzzer

	//infinite while loop that runs and implements all tasls	
	while (1) {
		for ( i = 0; i < 5; i++ ) {
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

