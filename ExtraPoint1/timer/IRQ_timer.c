/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../operations.h"

extern int timerMove;
extern int turnPlayer;
extern int currentPosPlayer1[2];
extern int currentPosPlayer2[2];
extern int modePlayer;
extern int tempPosWall[2];
extern int positionWall; //vertical or horizontal
extern int confirmWall;
extern int delta;
extern int spaceSquare;
extern int enableJoystick;
extern int spostato;
extern int confirmMove;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	NVIC_EnableIRQ(RIT_IRQn);
	//drawSecondsOnDislay();
	if(timerMove == 0){
		timerMove = 20; //reset of timer
		
		NVIC_DisableIRQ(EINT1_IRQn); //disable KEY1

		enableJoystick = 0; //disable joystick
		
		spostato = 0; //reset spostato
		
		//se l'utente non ha inserito in tempo il muro		
		if(modePlayer == 2 && confirmWall != 2) { 
			drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black); //viene nascosto
		}
		
		//reset mode player
		modePlayer = 1;
		
		//change turn of player
		if(turnPlayer == 1){
			//drawPlayer
			drawPlayer(turnPlayer, currentPosPlayer1[0], currentPosPlayer1[1]);
			//colorare di nero quadrati delle possibili mosse del player 1
			drawBlackSquares(currentPosPlayer1[0], currentPosPlayer1[1]);
			if(confirmWall != 2 && confirmMove == 0)
				//inserts move into vector
				insertPlayerMoves(currentPosPlayer1[0], currentPosPlayer1[1], turnPlayer);
			turnPlayer = 2;
			
			//coloro di bianco la linea del giocatore precedente (P1)
			LCD_DrawLine(12, 250, 12, 250+50, White); //linea verticale sinistra
			LCD_DrawLine(12, 250, 12+65, 250, White); //linea alta
			LCD_DrawLine(12+65, 250, 12+65, 250+50, White); //linea laterale destra
			LCD_DrawLine(12, 250+50, 12+65, 250+50, White); //linea bassa
			
			//coloro la linea per far capire a quale giocatore sta (P2)
			LCD_DrawLine(12+2*65+2*8, 250, 12+2*65+2*8, 250+50, Yellow); //linea verticale sinistra
			LCD_DrawLine(12+2*65+2*8, 250, 12+3*65+2*8, 250, Yellow); //linea alta
			LCD_DrawLine(12+3*65+2*8, 250, 12+3*65+2*8, 250+50, Yellow); //linea laterale destra
			LCD_DrawLine(12+2*65+2*8, 250+50, 12+3*65+2*8, 250+50, Yellow); //linea bassa
			
		}else{
			//drawPlayer
			drawPlayer(turnPlayer, currentPosPlayer2[0], currentPosPlayer2[1]);
			//colorare di nero quadrati delle possibili mosse del player 2
			drawBlackSquares(currentPosPlayer2[0], currentPosPlayer2[1]);
			if(confirmWall != 2 && confirmMove == 0)
				//inserts move into vector
				insertPlayerMoves(currentPosPlayer2[0], currentPosPlayer2[1], turnPlayer);
			turnPlayer = 1;
			
			//coloro di bianco la linea del giocatore precedente (P2)
			LCD_DrawLine(12+2*65+2*8, 250, 12+2*65+2*8, 250+50, White); //linea verticale sinistra
			LCD_DrawLine(12+2*65+2*8, 250, 12+3*65+2*8, 250, White); //linea alta
			LCD_DrawLine(12+3*65+2*8, 250, 12+3*65+2*8, 250+50, White); //linea laterale destra
			LCD_DrawLine(12+2*65+2*8, 250+50, 12+3*65+2*8, 250+50, White); //linea bassa
			
			//coloro la linea per far capire a quale giocatore sta (P1)
			LCD_DrawLine(12, 250, 12, 250+50, Yellow); //linea verticale sinistra
			LCD_DrawLine(12, 250, 12+65, 250, Yellow); //linea alta
			LCD_DrawLine(12+65, 250, 12+65, 250+50, Yellow); //linea laterale destra
			LCD_DrawLine(12, 250+50, 12+65, 250+50, Yellow); //linea bassa
			
		}
		//reset confirmWall
		confirmWall = 0;
		
		//reset confirmMove
		confirmMove = 0;
		
		//disegno le altre possibili mosse dell'altro giocatore
		drawPossibilyMoves(turnPlayer);
		
		enableJoystick = 1; //enable joystick
		NVIC_EnableIRQ(EINT1_IRQn); //enable KEY1
		drawSecondsOnDislay();
	}else if(timerMove > 0 && confirmMove == 0 && confirmWall != 2){
		drawSecondsOnDislay();
 		if(timerMove > 0)
			timerMove--;
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
