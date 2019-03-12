

#ifndef WORDOPERATIONS_H_
#define WORDOPERATIONS_H_

unsigned char wordToGuess[17] = {' '};
unsigned char ScoreArray[4] = {0};
	unsigned char hundreds;
	unsigned char thousands;
	unsigned char tens;
	unsigned char ones;



void ResetWelcomeMessage(unsigned char message[67]){
	
	unsigned char done = 0;
	unsigned char frontChar;
	
	while(!done){
		if(message[0] == 'L'){
			done = 1;
		}else{
			frontChar = message[0];
			
			for (unsigned char j = 0; j < 67; j++) {
				message[j] = message[j + 1];
			}
			// put the saved character on the end
			message[67] = frontChar;
		}
	}
}

void EEPROM_Write(unsigned int uiAddress, unsigned char ucData){
	
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

void ReturnHighScore(unsigned char newScore){
	thousands = newScore / 1000;
	hundreds = (newScore % 1000) / 100;
	 tens = (newScore % 100) / 10;
	ones = newScore % 10;
}



#endif /* WORDOPERATIONS_H_ */