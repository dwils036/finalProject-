/*
 * Array Test.c
 *
 * Created: 3/6/2020 1:02:29 PM
 * Author : david
 */ 

#include <avr/io.h>
#include <time.h>
#include "io.h"

//unsigned int k=1;
//unsigned char a1=0x00;

//enum trivSM{start, wait, reset, result, playerScore, noScore, playerWin} state;
	


const unsigned char* arrayFunction(unsigned char k) {//question array that corresponds with random value k.
	const unsigned char* message;
	
	switch (k) {
		case 0:
			message = "True";
			
			// char = message[0];
		break;
		
		case 1:
		message = "False";
		
		// char = message[0];
		break;
		
		case 2:
		message = "True";
		
		// char = message[0];
		break;
		
		case 3:
		message = "False";
		
		// char = message[0];
		break;
		
		case 4:
		message = "True";
		
		// char = message[0];
		break;
		
		case 5:
		message = "False";
		
		// char = message[0];
		break;
		
		case 6:
		message = "True";
		
		// char = message[0];
		break;
		
		case 7:
		message = "False";
		
		// char = message[0];
		break;
		
		case 8:
		message = "True";
		
		// char = message[0];
		break;
		
		case 9:
		message = "False";
		
		// char = message[0];
		break;
		
		case 10:
		message = "True";
		
		// char = message[0];
		break;
		
		case 11:
		message = "False";
		
		// char = message[0];
		break;
		
		case 12:
		message = "True";
		
		// char = message[0];
		break;
		
		case 13:
		message = "False";
		
		// char = message[0];
		break;
		
		case 14:
		message = "True";
		
		// char = message[0];
		break;
		
		case 15:
		message = "False";
		
		// char = message[0];
		break;
		
		case 16:
		message = "True";
		
		// char = message[0];
		break;
		
		case 17:
		message = "False";
		
		// char = message[0];
		break;
		
		case 18:
		message = "True";
		
		// char = message[0];
		break;
		
		case 19:
		message = "False";
		
		// char = message[0];
		break;
		
		default:
			message = "DEFAULT";
		break;
	}
	
	return message;
}
const unsigned char* arrayAnswer(unsigned char k) {//answer array used to determine true/false statements that correlates with randomized k
	const unsigned char* trueStatement;
	
	switch (k) {
		case 0:
		 trueStatement= 1;
		
		// char = message[0];
		break;
		
		case 1:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 2:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 3:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 4:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 5:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 6:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 7:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 8:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 9:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 10:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 11:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 12:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 13:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 14:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 15:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 16:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 17:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		case 18:
		trueStatement = 1;
		
		// char = message[0];
		break;
		
		case 19:
		trueStatement = 0;
		
		// char = message[0];
		break;
		
		default:
		trueStatement = 1;
		break;
	}
	
	return trueStatement;
}


/*void Trivia (){ //transitions
	switch (state){
		case start: 
		if (!a6)
		state = start;
	
		else if (a6)
		state=reset;
		break;
		
		case reset:
		 state=wait;
		 break;
		
		case wait:
		if (i>=20||a4&&a0||a4&&a1||a4&&a2||a4&&a3)
		state=result;
		else if(a5)
		state=reset;
		else
		state=wait;
		break;
		
		case result:
		if (a0&&trueStatement==1){
			if (totalScore1>=10){
			state=player1Win;}
		state=player1Score;
		}
		else if (a2&&trueStatement==1){
			if (totalScore2>=10){
			state=player2Win;}
			state=player2Score;
		}
		else if (a1&&trueStatement==1)
			state=noScore;
			
		else if (a3&&trueStatement==1)
			state=noScore;
		
		else if (a1&&trueStatement==0){
				if (totalScore1>=10){
				state=player1Win;
				}
				state=player1Score;
			}
		else if (a3&&trueStatement==0){
			if (totalScore2>=10){
				state=player2Win;
			}
			state=player2Score; 
		}
		else if (a5)
		state=reset;
		
		else
			state=noScore;
		break;
		
		case player1Score:
		state=wait;
		break;
		
		case player2Score:
		state=wait;
		break;
		
		case noScore:
		state=wait;
		break;
		
		case player1Win:
		if (!a5)
		state=player1Win;
		else
		state=reset;
		break;
		
		case player2Win:
		if (!a5)
		state=player2Win;
		else
		state=reset;
		break;
		
		
		default:
		break;
	}

	switch (state){//actions
		
				case start:
				
				break;
				
				case reset:
				totalScore1=0;
				totalScore2=0;
				break;
				
				case wait:
				LCD_DisplayString(1, arrayFunction(k));
				break;
				
				case result:
				arrayAnswer(k);
				break;
				
				case player1Score:
				happyTune0();
				totalScore1++;
				break;
				
				case player2Score:
				happyTune0();
				totalScore2++;
				break;
				
				case noScore:
				sadTune0();
				totalScore1=totalScore1+0;
				totalScore2=totalScore2+0
				break;
				
				case player1Win:
				ledDisplay2();
				happyTune1();
				break;
				
				case player2Win:
				ledDisplay2();
				happyTune1();
				break;
				
				
				
				default:
				break;
		
		
		
	}
}*/

int main(void) {
	
	DDRA = 0x00;    PORTA = 0xFF;
	DDRC = 0xFF;	PORTC = 0x00;
	DDRD = 0xFF;	PORTD = 0x00;
	
	time_t t;
	
	srand(time(NULL)); //NULL
	
	unsigned char k = rand() % 19;
	
	LCD_init();
	LCD_DisplayString(1, arrayFunction(k));		// Pass in k instead of 0 after adding cases for it in arrayFunction
	//a1=~PINA & 0x01;
    while (1) {
		
		//Trivia();
    }
	
	return 0;
}

