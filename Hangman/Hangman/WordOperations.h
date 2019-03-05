

#ifndef WORDOPERATIONS_H_
#define WORDOPERATIONS_H_

unsigned char wordToGuess[17] = {' '};
unsigned char wordLength = 0;

unsigned char* SizeOfWord(){
	unsigned char length = 0;
	unsigned char* length1;
	unsigned char index = 0;
	unsigned char found = 0;
	
	while(!found && (index < 17)){
		if(wordToGuess[index] == ' '){
			found = 1;
		}
		index++;
		length++;
	}
	length--;
	switch(length){
		case 0 : length1 = 'zero';
		break;
		case 1 : length1 = 'one';
		break;
		case 2 : length1 = 'two';
		break;
		case 3 : length1 = 'three';
		break;
		case 4 : length1 = 'four';
		break;
default:
		break;
	}
	
	return length;
}

#endif /* WORDOPERATIONS_H_ */