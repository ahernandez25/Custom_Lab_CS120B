/*  Name & E-mail: Ashly Hernandez ahern122@ucr.edu
 *	------------------------------------------------
 *  Contains the functions and task function to run the nokia screen. 
 */

#ifndef NOKIALCD_H_
#define NOKIALCD_H_


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "SPI_Master_H_file.h"
#include "Font.h"
#include "LCDImages.h"


/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_Cmnd(char DATA)
{
	PORTB &= ~(1<<DC);				/* make DC pin to logic zero for command operation */
	SPI_SS_Enable();				/* enable SS pin to slave selection */
	SPI_Write(DATA);				/* send data on data register */
	PORTB |= (1<<DC);				/* make DC pin to logic high for data operation */
	SPI_SS_Disable();
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_Data(char *DATA)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();									/* enable SS pin to slave selection */
	int lenan = strlen(DATA);							/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			SPI_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */
		}
		SPI_Write(0x00);
	}
	SPI_SS_Disable();
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_Reset()					/* reset the Display at the beginning of initialization */
{
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_init()
{
	N5110_Reset();					/* reset the display */
	N5110_Cmnd(0x21);				/* command set in addition mode */
	N5110_Cmnd(0xC0);				/* set the voltage by sending C0 means VOP = 5V */
	N5110_Cmnd(0x07);				/* set the temp. coefficient to 3 */
	N5110_Cmnd(0x13);				/* set value of Voltage Bias System */
	N5110_Cmnd(0x20);				/* command set in basic mode */
	N5110_Cmnd(0x0C);				/* display result in normal mode */
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void lcd_setXY(char x, char y)		/* set the column and row */
{
	N5110_Cmnd(x);
	N5110_Cmnd(y);
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_clear()					/* clear the Display */
{
	SPI_SS_Enable();
	PORTB |= (1<<DC);
	for (int k=0; k<=503; k++)
	{
		SPI_Write(0x00);
	}
	PORTB &= ~(1<<DC);
	SPI_SS_Disable();
}

/*  
   Nokia Display N5110 Interfacing with ATmega 
   http://www.electronicwings.com
*/
void N5110_image(const unsigned char *image_data)		/* clear the Display */
{
	SPI_SS_Enable();
	PORTB |= (1<<DC);
	for (int k=0; k<=503; k++)
	{
		SPI_Write(image_data[k]);
	}
	PORTB &= ~(1<<DC);
	SPI_SS_Disable();
}

//Declares states for Nokia screen task
enum NokiaStates {Nokia_Init, Nokia_Wait, Strike1, Strike2, Strike3, 
	Strike4, Strike5, Nokia_Lose, Nokia_Win, Nokia_Reset};

unsigned char strike = 0;		//counts number of times user guessed incorrect letter
unsigned char counter = 0;		//counts ticks to display screen for
unsigned char NOKIAReset = 0;	//reset flag - tells screen to reset
	
int Nokia_Tick(int state){
	switch (state)
	{
		//initializes screen, sets it with the logo, and sets next state to Nokia_Wait
		case Nokia_Init: 	SPI_Init();
							N5110_init();
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&logo);
							state = Nokia_Wait;
		break;
		
		/* checks for reset. if not, sets state to strike 1 if answer is wrong. else it 
		 *  stays in wait state*/		
		case Nokia_Wait :	if(NOKIAReset){
								state = NOKIAReset;
								strike = 0;
								counter = 0;
								NOKIAReset = 0;
							}else if(strike == 1){
							 state = Strike1;
							 
							 //clears screen and displays head
							 N5110_clear();
							 lcd_setXY(0x40,0x80);
							 N5110_image(&head);
						  } else{
							  state = Nokia_Wait;
						  } 
		break;
		
		/* checks for reset. if not, sets state to strike 2 if answer is wrong. else it 
		 *  stays in strike1 state*/
		case Strike1 :	if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(strike == 2){
							state = Strike2;
							//clears screen then displays head and body
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body);
						} else{
							state = Strike1;
						}
		break;
		
		/* checks for reset. if not, sets state to strike 3 if answer is wrong. else it 
		 *  stays in strike2 state */
		case Strike2 :  if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(strike == 3){
							state = Strike3;
							
							//clears screen and displays head, body and 1 arm
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm1);
						} else{
							state = Strike2;
						}
		break;
		
		/* checks for reset. if not, sets state to strike4 if answer is wrong. else it 
		 *  stays in strike3 state */
		case Strike3 :	if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(strike == 4){
							state = Strike4;
							//clears screen and displays head, body and 2 arms
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm2);
						} else{
							state = Strike3;
						}
		break;
		
		/* checks for reset. if not, sets state to strike5 if answer is wrong. else it 
		 *  stays in strike4 state */
		case Strike4 :  if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(strike == 5){
							state = Strike5;
							
							//clears screen and displays head, body, ars, and 1 leg
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm_leg1);
						} else{
							state = Strike4;
						}	
		break;
		
		/* checks for reset. if not, sets state to Nokia_Lose if answer is wrong. else it 
		 *  stays in strike5 state */
		case Strike5 :  if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(strike == 6){
							state = Nokia_Lose;
							N5110_clear();
		 					lcd_setXY(0x40,0x80);
							 //sets screen to full stick figure
		 					N5110_image(&head_body_arm_leg2);
						} else{
							state = Strike5;
						}
		break;
		
		/* checks for reset. if not, stays in nokia_lose for 20 ticks. then state is 
		 * set to nokia_wait */
		case Nokia_Lose :	if(NOKIAReset){
								state = NOKIAReset;
								strike = 0;
								counter = 0;
								NOKIAReset = 0;
							}else if(counter <= 20){
								state = Nokia_Lose;
							}else if(counter > 20)
							{
								state = Nokia_Wait;
							}
		break;
		
		/* checks for reset. if not, stays in state for 20 ticks. 
		 * then state get set to nokia_wait*/
		case Nokia_Win : if(NOKIAReset){
							state = NOKIAReset;
							strike = 0;
							counter = 0;
							NOKIAReset = 0;
						}else if(counter <= 20){
							state = Nokia_Win;
						}else if(counter > 20)
						{
							state = Nokia_Wait;
						}
		break;
		
		/* state transitions to nokia_init */
		case Nokia_Reset : state = Nokia_Init;
		break;
	}//end transitions
	
	switch (state)
	{
		case Nokia_Init :
		break;
		case Nokia_Wait : 
		break;
		case Strike1 :	
		break;
		case Strike2 :  
		break;
		case Strike3 :	
		break;
		case Strike4 : 
		break;
		case Strike5 : 
		break;
		case Nokia_Lose :	//clears screen on even ticks
							if(counter % 2 == 0){
								N5110_clear();
							}
							//displays stick figure on odd ticks
							else
							{
								lcd_setXY(0x40, 0x80);
								N5110_image(head_body_arm_leg2);
							}
							counter++;
		break;
		case Nokia_Win :
		break;
		case Nokia_Reset :	
		break;
	}
	
	return state;
}


#endif /* NOKIALCD_H_ */