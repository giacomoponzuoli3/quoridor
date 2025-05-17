/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


//vettore mosse
volatile uint32_t moves[200];
volatile int i = 0;

//muri rimanenti da inserire per i 2 player
volatile int wallsPlayer1 = 8;
volatile int wallsPlayer2 = 8;

//timer move
volatile int timerMove = 20;

//dim square
volatile const int delta = 25;

//space square
volatile const int spaceSquare = 5;

/***************************
* turnPlayer : variabile che indica quale è il player che deve giocare
* 1 : player 0
* 2: player 1
****************************/
volatile int turnPlayer;

/********************************

********************************/


//current position of 2 players
volatile int currentPosPlayer1[2];
volatile int currentPosPlayer2[2];

//temp save the position of player 
volatile int tempPosPlayer[2];

/*******************************
* confirmMove 
* 0 : non ha deciso di confermare la nuova posizione
* 1 : ha deciso di confermare la nuova posizione
********************************/
volatile int confirmMove = 0;


/**************************************
* temp save the position of player's wall
* tempPosWall[0] : riga del centro del muro
* tempPosWall[1] : colonna del centro del muro
***************************************/
volatile int tempPosWall[2]; 


/***********************************************************
temp save the mode vertical or horizontal of position of wall
* - 1 : vertical 
* - 2 : horizontal
**************************************************************/
volatile int positionWall;

//conferma se il muro deve essere salvato o no
volatile int confirmWall;

//matrix 
volatile int m[13][13];

/***********************************
modePlayer:
- 1 = move player
- 2 = move wall of player
***************************/
volatile int modePlayer = -1;

/*****************************
* enableJoystick: variabile che ci permette di attivare/disattivare l'uso del joystick
* 0 : disable
* 1 : enable
****************************/
volatile int enableJoystick = 0; //at the start


/************************************************************
* visited[13][13]
* matrice che va ad informare quali celle per le pedine posso visitare 
* viene utilizzata nella dfs quando dobbiamo controllare se il muro che andiamo ad inserire blocca l'altro avversario o no
************************************************************/
volatile int visited[13][13];



int main(void)
{
	int i, j;
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();
  LCD_Initialization();
	TP_Init();
	joystick_init();
		
	//initialization matrix
	for(i = 0; i < 13; i++)
		for(j = 0; j < 13; j++)
			m[i][j] = 0;
	
	TouchPanel_Calibrate();
	
	LCD_Clear(Black);	
	
	LCD_DrawScacchiera(delta, spaceSquare, White);
	LCD_DrawInformation(65, 50, White); 
	
	NVIC_EnableIRQ(EINT0_IRQn);
	
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
