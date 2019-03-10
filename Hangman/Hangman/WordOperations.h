

#ifndef WORDOPERATIONS_H_
#define WORDOPERATIONS_H_

unsigned char wordToGuess[17] = {' '};

unsigned char wordLength = 0;


int SizeOfWord(){
	int length = 0;
	//unsigned char* length1;
	unsigned char found = 0;
	
	while(!found && (index < 17)){
		if(wordToGuess[index] == ' '){
			found = 1;
		}
		else{
			index++;
			length++;
		}
		
	}
	
	
	
	return length;
}





#endif /* WORDOPERATIONS_H_ */