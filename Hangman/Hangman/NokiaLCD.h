/*
 * NokiaLCD.h
 *
 * Created: 2/25/2019 8:48:09 PM
 *  Author: ashly
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


void N5110_Cmnd(char DATA)
{
	PORTB &= ~(1<<DC);				/* make DC pin to logic zero for command operation */
	SPI_SS_Enable();				/* enable SS pin to slave selection */
	SPI_Write(DATA);				/* send data on data register */
	PORTB |= (1<<DC);				/* make DC pin to logic high for data operation */
	SPI_SS_Disable();
}

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

void N5110_Reset()					/* reset the Display at the beginning of initialization */
{
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

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

void lcd_setXY(char x, char y)		/* set the column and row */
{
	N5110_Cmnd(x);
	N5110_Cmnd(y);
}

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

void N5110_Custom_Data(const unsigned char *data)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();		
								/* enable SS pin to slave selection */
	unsigned char yPos = 0x40;
	unsigned char xPos = 0x80;
	//lcd_setXY(yPos, 0x80);
	for (int g = 0; g < 504; g++)
	{
// 		if(((g % 3) == 0) && (g != 0) ){
// 			yPos = yPos + 8;
// 			xPos = xPos + 5;
// 			lcd_setXY(yPos, xPos);
// 		}
		
		//for (int index = 0; index < 5; index++)
		//{
			SPI_Write(data[g]);	/* send the data on data register */
		//}
		SPI_Write(0x00);
	}
	SPI_SS_Disable();
}

enum NokiaStates {Nokia_Init, Nokia_Wait, Strike1, Strike2, Strike3, Strike4, Strike5, Nokia_Lose, 
	Nokia_Win};

unsigned char strike = 0;
	
int Nokia_Tick(int state){
	switch (state)
	{
		case Nokia_Init: 	SPI_Init();
							N5110_init();
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&logo);
							state = Nokia_Wait;
		break;
		case Nokia_Wait : if(strike == 1){
							 state = Strike1;
							 N5110_clear();
							 lcd_setXY(0x40,0x80);
							 N5110_image(&head);
						  } else{
							  state = Nokia_Wait;
						  } 
		break;
		case Strike1 :	if(strike == 2){
							state = Strike2;
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body);
						} else{
							state = Strike1;
						}
		break;
		case Strike2 :  if(strike == 3){
							state = Strike3;
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm1);
						} else{
							state = Strike2;
						}
		break;
		case Strike3 :	if(strike == 4){
							state = Strike4;
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm2);
						} else{
							state = Strike3;
						}
		break;
		case Strike4 : if(strike == 5){
							state = Strike5;
							N5110_clear();
							lcd_setXY(0x40,0x80);
							N5110_image(&head_body_arm_leg1);
						} else{
							state = Strike4;
						}	
		break;
		case Strike5 :  if(strike == 6){
							state = Nokia_Lose;
							N5110_clear();
		 					lcd_setXY(0x40,0x80);
		 					N5110_image(&head_body_arm_leg2);
						} else{
							state = Strike5;
						}
		break;
		case Nokia_Lose :
		break;
		case Nokia_Win :
		break;
	}//end transitions
	
	switch (state)
	{
		case Nokia_Init :
		break;
		case Nokia_Wait : 
		break;
		case Strike1 :	//N5110_clear();
		break;
		case Strike2 :  
		break;
		case Strike3 :	
		break;
		case Strike4 : 
		break;
		case Strike5 : 
		break;
		case Nokia_Lose :
		break;
		case Nokia_Win :
		break;
	}
	
	return state;
}


#endif /* NOKIALCD_H_ */