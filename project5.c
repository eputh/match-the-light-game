/*
 * project5.c
 *
 * Created: 6/2/2015 1:57:03 PM
 *  Author: Emily Puth and Chelsea Faustino
 */ 


#include <avr/io.h>
#include<stdbool.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"


void wait_whatever(int speed) {
	for (volatile unsigned int i=0; i<speed; i++) {}
}

// function to loop through light sequence and check keypad pressed
int playerTurn (int speed) {
	char turn = "Your Turn";
	pos_lcd(1,0);
	puts_lcd2(turn);
	
	int playerLight;
	for (int forward=0; forward<8; forward++) {
		SET_BIT(PORTA, forward);
		if (get_key() == 13) {
			playerLight = forward;
			return playerLight;
		}
		else {
			playerLight = 8;
		}
		wait_whatever(speed);
		CLR_BIT(PORTA, forward);
	}
	for (int backward=6; backward>0; backward--) {
		SET_BIT(PORTA, backward);
		if (get_key() == 13) {
			playerLight = backward;
			break;
		}
		else {
			playerLight = 8;
		}
		wait_whatever(speed);
		CLR_BIT(PORTA, backward);
	}
	return playerLight;
}


// function to pick random light (computer turn)
int computerTurn(int speed) {
	char turn = "Computer's Turn";
	pos_lcd(1,0);
	puts_lcd2(turn);
	
	int compLight = rand() % 7; // generate random num to light random LED
	for (int i=0; i<10; i++) {
		SET_BIT(PORTA, compLight);
		wait_whatever(speed);
		CLR_BIT(PORTA, compLight);
		wait_whatever(speed);
	}
	return compLight;
}

// check gameOver: turn off all LEDs, clear LCD and print results
void gameOver(int playerScore, int compScore) {
	char winner;
	char result [15];
	if (playerScore > compScore)
		winner = "YOU WIN!";
	else
		winner = "COMPUTER WINS";
	sprintf(result, "%s", winner);
	clr_lcd();
	pos_lcd(0,0);
	puts_lcd2(result);
}

void updateScore(int playerScore, int compScore) {
	char scoreboard[15];
	sprintf(scoreboard, "PLY:%d CPU:%d", playerScore, compScore);
	pos_lcd(0,0);
	puts_lcd2(scoreboard);
}

// update scores and turn
void updateLCD(int playerScore, int compScore) {
	clr_lcd();
	updateScore(playerScore, compScore);
}

int main(void)
{
	ini_lcd();
	clr_lcd();
	
	DDRA = 0xFF;
	PORTA = 0x00;
	
	int playerScore = 0;
	int compScore = 0;
	int turnCount = 0;
	
	char random[15];
	int compLight = 0; // chosen light
	int playerLight = 8;
	int compTurn = 1; // boolean to check which turn
	
	int inGame = 0;
	int speed = 60000;
	int lightOn = 60000;
	
	while(1)
    {	 
		// play multiple times
		if (inGame == 0) {
			// start screen
			char startScreen = "Press D to start";
			pos_lcd(1,0);
			puts_lcd2(startScreen);
			
			if (get_key() == 16) {
				inGame = 1;
			}
		} else {
			updateLCD(playerScore, compScore);
			
			// if game over, break out of game-play loop
			if (compScore == 5 || playerScore == 5) {
				gameOver(playerScore, compScore);
				
				playerScore = 0;
				compScore = 0;
				playerLight = 8;
				speed = 60000;
				inGame = 0;
		
			//check turns
			} else if (compTurn) {
				// Computer chooses a random LED light
				compLight = computerTurn(speed);	
				compTurn = 0;	
			} else {
				// LEDs light up one at a time
				playerLight = playerTurn(speed);
			
				// if button pressed, update score and stop bouncing lights
				if (playerLight < 8) {
					if (playerLight == compLight) {
						playerScore++;
						speed -= 10000;
					} else {
						compScore++;
					}
					updateLCD(playerScore, compScore);
						
					// blink player's light
					for (int i = 0; i < 5; i++) {
						SET_BIT(PORTA, playerLight);
						wait_whatever(lightOn);
					}
					CLR_BIT(PORTA, playerLight);
					wait_whatever(lightOn);
					compTurn = 1; 
				} // end if : bounce lights
			} // end if : check game over / turns
		} // end if : replay game 
	} // end while
} // end main
