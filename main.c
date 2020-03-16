/*David Wilson (862134618) dwils036@ucr.edu
 * Partner: none
 * Lab Section: 023
 * Assignment: Final Project
 * Trivia Game Show
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

// Function Prototypes
const unsigned char* arrayFunction(unsigned char);
unsigned char arrayAnswer(unsigned char);

// Global Variables
unsigned char a0;
unsigned char a1;
unsigned char a2;
unsigned char a3;
unsigned char a4;
unsigned char a5;
unsigned char a6;
unsigned char a7;
unsigned char answer;

// Shared Variables - they are global as well
unsigned char flagLED = 0;
unsigned char flagLEDR = 0;
unsigned char flagSpeaker = 0;
unsigned char output = 0;
unsigned char tasksNum = 4;
unsigned char secCount = 0;
unsigned char randTick = 0;

// Timer Manager
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


void TimerOn() {
	
	TCCR1B 	= 0x0B;
	
	OCR1A 	= 125;

	TIMSK1 	= 0x02;

	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;	// 0x80=1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // 0x00=off
}

typedef struct _task{
	// Tasks should have members that include: state, period,
	//a measurement of elapsed time, and a function pointer.
	signed   char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

task tasks[4];
//period 
unsigned long triviaPeriod = 50;
unsigned long LEDWinPeriod = 300;
unsigned long LEDNoScorePeriod = 200;
unsigned long speakerPeriod = 50;
unsigned long randTickPeriod = 1;
unsigned long GCD = 1;

enum trivSM { wait0, wait1, reset, wait2, player1Score, player2Score, noScore, player1Win, player2Win } state;
int Trivia (int state);
enum ThreeStateMachine { threeLEDS, State1, State2, State3 } state3LEDS;
int ThreeLEDs (int state3LEDS);
enum Wrong {oneLED, stateON0, stateOFF0, stateON1, stateOFF1} stateRed;
int redLight (int stateRed);
enum randSM{ rState } randState;
int randGen (int randState);
/*enum speakerSM {speakerStart} speakerState;
int speaker (int speakerState);*/


void TimerISR() {
	unsigned char i;
	for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
		if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += GCD;
	}
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

int main(void) {
	
	DDRA = 0x00;    PORTA = 0xFF;	//input
	DDRB = 0xFF;	PORTB = 0x00;	//output
	DDRC = 0xFF;	PORTC = 0x00;	//output
	DDRD = 0xFF;	PORTD = 0x00;	//output
	
	unsigned char x = 0;
	tasks[x].state = reset;
	tasks[x].period = triviaPeriod;
	tasks[x].elapsedTime = tasks[x].period;
	tasks[x].TickFct = &Trivia;
	x++;
	tasks[x].state = threeLEDS;
	tasks[x].period = LEDWinPeriod;
	tasks[x].elapsedTime = tasks[x].period;
	tasks[x].TickFct = &ThreeLEDs;
	x++;
	tasks[x].state = oneLED;
	tasks[x].period = LEDNoScorePeriod;
	tasks[x].elapsedTime = tasks[x].period;
	tasks[x].TickFct = &redLight;
	x++;
	/*tasks[x].state = speakerStart;
	tasks[x].period = speakerPeriod;
	tasks[x].elapsedTime = tasks[x].period;
	tasks[x].TickFct = &speaker;
	x++;*/
	tasks[x].state = randState;
	tasks[x].period = randTickPeriod;
	tasks[x].elapsedTime = tasks[x].period;
	tasks[x].TickFct = &randGen;
	
	TimerSet(GCD);
	TimerOn();

	LCD_init();

    while (1) {
		
		unsigned char tmpA = ~PINA;
		
		a0 = tmpA & 0x01;
		a1 = tmpA & 0x02;
		a2 = tmpA & 0x04;
		a3 = tmpA & 0x08;
		a4 = tmpA & 0x10;
		a5 = tmpA & 0x20;
		a6 = tmpA & 0x40;
		a7 = tmpA & 0x80;
		answer = arrayAnswer(randTick);
		
	}
	
	return 0;
}

int randGen(int randState) {
	switch(randState) {
		case rState:
			if(randTick > 19) randTick = 0;
			else randTick++;
			randState = rState;
		break;
	}
	
	return randState;
}
unsigned char* msg;
unsigned char trueStatement;
unsigned char totalScore1 = 0;
unsigned char totalScore2 = 0;
int Trivia (int state) {
	
	switch (state) {
	
		case reset:
			totalScore1 = 0;
			totalScore2 = 0;
			state = wait0;
		break;
		
		case wait0:
			secCount = 0;
			LCD_DisplayString(1, arrayFunction(randTick));
			trueStatement = arrayAnswer(randTick);
			state = wait1;
		break;
		
		case wait1:
			if(secCount < 400) secCount++;
			if (secCount >= 400 || (a4 && a0) || (a4 && a1) || (a4 && a2) || (a4 && a3)) {
				if (a0 && trueStatement) {
					if (totalScore1 >= 10) {
						state = player1Win;
					}
					else {
						state = player1Score;
						secCount = 0;
					}
				}
				else if (a2 && trueStatement) {
					if (totalScore2 >= 10) {
						state = player2Win;
					}
					else {
						state = player2Score;
						secCount = 0;
					}
				}
				else if (a1 && !trueStatement) {
					if (totalScore1 >= 10) {
						state = player1Win;
					}
					else {
						state = player1Score;
						secCount = 0;
					}
						
				}
				else if (a3 && !trueStatement) {
					if (totalScore2 >= 10) {
						state = player2Win;
					}
					else {
						state = player2Score;
						secCount = 0;
					}
				}
				else if (a0 && !trueStatement) {
					secCount = 0;
					state = noScore;
				}
				else if (a2 && !trueStatement) {
					secCount = 0;
					state = noScore;
				}
				else if (a1 && trueStatement) {
					secCount = 0;
					state = noScore;
				}
				else if (a3 && trueStatement) {
					secCount = 0;
					state = noScore;
				}
				else state = wait0;
			}
			else if(a5)
				state = reset;
			else
				state = wait1;
		break;
		
		case wait2:
			if(!a0 && !a1 && !a2 && !a3) state = wait0;
			else state = wait2;
		break;
		
		case player1Score:
			if(!secCount) {
				totalScore1++;
				if(totalScore1 < 10 && totalScore2 < 10) {
					msg = "Player 1:       Player 2:  ";  // Score goes at positions 10 and 26
					msg[10] = totalScore1 + '0';
					msg[26] = totalScore2 + '0';
				}
				else if(totalScore1 >= 10 && totalScore2 < 10) {
					msg = "Player 1 WINS!!!";
				}
				else if(totalScore1 < 10 && totalScore2 >= 10) {
					msg = "Player 2 WINS!!!";
				}
				LCD_DisplayString(1, msg);
				flagLED = 1;
			}
			if(secCount >= 50) {
				secCount = 0;
				state = wait2;
			}
			else {
				secCount++;
				state = player1Score;
			}
		break;
		
		case player2Score:
			//happyTune0();
			if(!secCount) {
				totalScore2++;
				if(totalScore1 < 10 && totalScore2 < 10) {
					msg = "Player 1:       Player 2:  ";  // Score goes at positions 10 and 26
					msg[10] = totalScore1 + '0';
					msg[26] = totalScore2 + '0';
				}
				else if(totalScore1 >= 10 && totalScore2 < 10) {
					msg = "Player 1 WINS!!!";
				}
				else if(totalScore1 < 10 && totalScore2 >= 10) {
					msg = "Player 2 WINS!!!";
				}
				LCD_DisplayString(1, msg);
				flagLED = 1;
			}
			if(secCount >= 50) {
				secCount = 0;
				state = wait2;
			}
			else {
				secCount++;
				state = player2Score;
			}
		break;
		
		case noScore:
			//sadTune0();
			if(!secCount) {
				msg = "Player 1:       Player 2:  ";  // Score goes at positions 10 and 26
				msg[10] = totalScore1 + '0';
				msg[26] = totalScore2 + '0';
				LCD_DisplayString(1, msg);
				flagLEDR = 1;
			}
			if(secCount >= 50) {
				secCount = 0;
				state = wait2;
			}
			else {
				secCount++;
				state = player2Score;
			}
		break;
		
		case player1Win:
			//ledDisplay2();
			//happyTune1();
			flagSpeaker = 1;
			if (!a5 && !flagSpeaker)
				state = player1Win;
			else
				state = reset;
		break;
		
		case player2Win:
			//ledDisplay2();
			//happyTune1();
			flagSpeaker = 1;
			if (!a5 && !flagSpeaker)
			state = player2Win;
			else
			state = reset;
		break;
		
		default:
		break;
	}
		
	return state;
}


const unsigned char* arrayFunction(unsigned char randTick) {//question array that corresponds with random value randTick.
	const unsigned char* message;
	
	switch (randTick) {
		case 0:
		message = "True1";
		break;
		
		case 1:
		message = "False1";
		break;
		
		case 2:
		message = "True2";
		break;
		
		case 3:
		message = "False2";
		break;
		
		case 4:
		message = "True3";
		break;
		
		case 5:
		message = "False3";
		break;
		
		case 6:
		message = "True4";
		break;
		
		case 7:
		message = "False4";
		break;
		
		case 8:
		message = "True5";
		break;
		
		case 9:
		message = "False5";
		break;
		
		case 10:
		message = "True6";
		break;
		
		case 11:
		message = "False6";
		break;
		
		case 12:
		message = "True7";
		break;
		
		case 13:
		message = "False7";
		break;
		
		case 14:
		message = "True8";
		break;
		
		case 15:
		message = "False8";
		break;
		
		case 16:
		message = "True9";
		break;
		
		case 17:
		message = "False9";
		break;
		
		case 18:
		message = "True10";
		break;
		
		case 19:
		message = "False10";
		break;
		
		default:
		message = "True11";
		break;
	}
	
	return message;
}
unsigned char arrayAnswer(unsigned char randTick) {//answer array used to determine true/false statements that correlates with randomized randTick
	unsigned char trueStatement;
	
	switch (randTick) {
		case 0:
		trueStatement = 1;
		break;
		
		case 1:
		trueStatement = 0;
		break;
		
		case 2:
		trueStatement = 1;
		break;
		
		case 3:
		trueStatement = 0;
		break;
		
		case 4:
		trueStatement = 1;
		break;
		
		case 5:
		trueStatement = 0;
		break;
		
		case 6:
		trueStatement = 1;
		break;
		
		case 7:
		trueStatement = 0;
		break;
		
		case 8:
		trueStatement = 1;
		break;
		
		case 9:
		trueStatement = 0;
		break;
		
		case 10:
		trueStatement = 1;
		break;
		
		case 11:
		trueStatement = 0;
		break;
		
		case 12:
		trueStatement = 1;
		break;
		
		case 13:
		trueStatement = 0;
		break;
		
		case 14:
		trueStatement = 1;
		break;
		
		case 15:
		trueStatement = 0;
		break;
		
		case 16:
		trueStatement = 1;
		break;
		
		case 17:
		trueStatement = 0;
		break;
		
		case 18:
		trueStatement = 1;
		break;
		
		case 19:
		trueStatement = 0;
		break;
		
		default:
		trueStatement = 1;
		break;
	}
	
	return trueStatement;
}

int ThreeLEDs(int state3LEDS) {
	switch(state3LEDS){
		case threeLEDS:
			output = 0x00;
			if(flagLED) state3LEDS = State1;
			else state3LEDS = threeLEDS;
		break;
		
		case State1:
			output = 0x01;
			state3LEDS = State2;
		break;
		
		case State2:
			output = 0x02;
			state3LEDS= State3;
		break;
		
		case State3:
			output = 0x04;
			flagLED = 0;
			state3LEDS = threeLEDS;
		break;
		
		default:
		break;
		
		
	}
	PORTB = output;
	return state3LEDS;
}
unsigned char y = 0;
/*int speaker (int speakerState) {
	switch (speakerState);
	case speakerStart:
		if (y==0){
			pwm_on ();
			set_pwm (523.25);
		}
		else if (y<=1||y<20){
			set_pwm (493.88);
		}
		else if (y<=20||y<40){
			set_pwm (523.25);
		}else if (y<=1||y<20){
			set_pwm (493.88);
	}
	y++
	PORTB = 0x40;
	break;
	
	default:
	break;
		}
	return speakerState;
}*/

int redLight(stateRed){
	switch (stateRed) {
		case oneLED:
			if(flagLEDR){
				 stateRed = stateOFF0;
				 output = 0x08;
			}
			else stateRed = oneLED;
		break;
		
		case stateOFF0:
			output = 0x00;
			stateRed = stateON0;
		break;
		
		case stateON0:
			output = 0x08;
			stateRed = stateOFF1;
		break;
		
		case stateOFF1:
			output = 0x00;
			stateRed = stateON1;
		break;
		
		case stateON1:
			output = 0x08;
			stateRed = oneLED;
			flagLEDR = 0;
		break;
		
		default:
		break;
	}
	PORTB = output;
	return stateRed;
}
		
		
	
		
	