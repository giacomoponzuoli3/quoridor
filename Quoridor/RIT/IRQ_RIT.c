/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../operations.h"

extern int turnPlayer; //turno del player
extern int modePlayer; //modalità player (wall or move)
extern int wallsPlayer1;
extern int wallsPlayer2;
extern int currentPosPlayer1[2];
extern int currentPosPlayer2[2];
extern int tempPosWall[2];
extern int confirmWall;
extern int positionWall;
extern int timerMove;
extern int delta;
extern int spaceSquare;
extern int m[13][13];
extern int enableJoystick;
extern int tempPosPlayer[2];
extern int confirmMove;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

extern char led_value;

int tempCol, tempRow;
int tempColSelect, tempRowSelect;

/********************************
* spostato : variabile che informa se ho già spostato
* 0 : ancora non ho spostato
* 1 : già spostato
********************************/
int spostato = 0;


void RIT_IRQHandler (void)
{					
	static int J_select = 0;
	static int J_down = 0;
	static int J_left = 0;
	static int J_right = 0;
	static int J_up = 0;
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick J_Select pressed p1.25*/
		
		J_select++;
		switch(J_select){
			case 1:
 				if(spostato == 1 && enableJoystick == 1){
					confirmMove = 1; //confermo la mossa
					GUI_Text(44+65, 270, (uint8_t *) "oo", Black, Black);
					if(turnPlayer == 1){
						tempRowSelect = currentPosPlayer1[0];
						tempColSelect = currentPosPlayer1[1];
						
						//inserisco la mossa del player
						insertPlayerMoves(tempPosPlayer[0], tempPosPlayer[1], turnPlayer);
						//reset matrix
						m[tempRowSelect][tempColSelect] = 0;
						
						if(currentPosPlayer1[0] == 12){ //vittoria
							reset_timer(0);
							disable_timer(0);
							disable_RIT();
							NVIC_DisableIRQ(EINT1_IRQn);
							NVIC_DisableIRQ(EINT2_IRQn);
							enableJoystick = 0;
							
							LCD_Clear(Black);
							GUI_Text(50, 150, (uint8_t *) "Player1 ha vinto!", White, Black);
							break;
						}
							
						
						//oscuro le possibili mosse
						drawBlackSquares(tempRowSelect, tempColSelect);
					}else if(turnPlayer == 2){
						tempRowSelect = currentPosPlayer2[0];
						tempColSelect = currentPosPlayer2[1];
						//inserisco la mossa del player
						insertPlayerMoves(tempPosPlayer[0], tempPosPlayer[1], turnPlayer);
						//reset matrix
						m[tempRowSelect][tempColSelect] = 0;
						//oscuro le possibili mosse
						drawBlackSquares(tempRowSelect, tempColSelect);
						
						if(currentPosPlayer2[0] == 0){ //vittoria
							reset_timer(0);
							disable_timer(0);
							disable_RIT();
							NVIC_DisableIRQ(EINT1_IRQn);
							NVIC_DisableIRQ(EINT2_IRQn);
							enableJoystick = 0;
							
							LCD_Clear(Black);
							
							GUI_Text(50, 150, (uint8_t *) "Player2 ha vinto!", White, Black);
							break;
						}
						
					}

					enableJoystick = 0; //disable joystick
					timerMove = 0; //interrupt timer
				}
				break;
			default:
				break;
		}
	}
	else{
			J_select=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick J_Down pressed p1.26 --> using J_DOWN due to emulator issues*/
		
		J_down++;
		switch(J_down){
			case 1:
				if(turnPlayer == 1 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer1[0];
						tempCol = currentPosPlayer1[1]; 
						if(spostato == 1 && (tempPosPlayer[0]+2 == currentPosPlayer1[0] || tempPosPlayer[0]+4 == currentPosPlayer1[0])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], White); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempRow+2 < 13 && m[tempRow+1][tempCol] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow+2][tempCol] == 1 && m[tempRow+3][tempCol] == 0 && tempRow+4 < 13){
								tempPosPlayer[0] = tempRow+4;
								tempPosPlayer[1] = tempCol;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}else if(m[tempRow+2][tempCol] == 0){
								tempPosPlayer[0] = tempRow+2;
								tempPosPlayer[1] = tempCol;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[0] < 11){ //se il muro non esce dalla scacchiera
							
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la riga del nuovo muro
							tempPosWall[0] = tempPosWall[0]+2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}else if(turnPlayer == 2 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer2[0];
						tempCol = currentPosPlayer2[1];
						if(spostato == 1 && (tempPosPlayer[0]+2 == currentPosPlayer2[0] || tempPosPlayer[0]+4 == currentPosPlayer2[0])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Red); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempRow+2 < 13 && m[tempRow+1][tempCol] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow+2][tempCol] == 1 && m[tempRow+3][tempCol] == 0 && tempRow+4 < 13){
								tempPosPlayer[0] = tempRow+4;
								tempPosPlayer[1] = tempCol;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}else if(m[tempRow+2][tempCol] == 0){
								tempPosPlayer[0] = tempRow+2;
								tempPosPlayer[1] = tempCol;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[0] < 11){ //se il muro non esce dalla scacchiera
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la riga del nuovo muro
							tempPosWall[0] = tempPosWall[0]+2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick J_Left pressed p1.27*/
		J_left++;
		switch(J_left){
			case 1:
				if(turnPlayer == 1 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						
						tempRow = currentPosPlayer1[0];
						tempCol = currentPosPlayer1[1];
						if(spostato == 1 && (tempPosPlayer[1]-2 == currentPosPlayer1[1] || tempPosPlayer[1]-4 == currentPosPlayer1[1])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], White); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempCol-2>=0 && m[tempRow][tempCol-1] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow][tempCol-2] == 1 && m[tempRow][tempCol-3] == 0 && tempCol-4 >= 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol-4;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
								
							}else if(m[tempRow][tempCol-2] == 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol-2;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[1] > 1){ //se il muro non esce dalla scacchiera
							
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la colonna del nuovo muro verso sinistra
							tempPosWall[1] = tempPosWall[1]-2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}else if(turnPlayer == 2 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer2[0];
						tempCol = currentPosPlayer2[1];
						
						if(spostato == 1 && (tempPosPlayer[1]-2 == currentPosPlayer1[1] || tempPosPlayer[1]-4 == currentPosPlayer1[1])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Red); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempCol-2>=0 && m[tempRow][tempCol-1] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow][tempCol-2] == 1 && m[tempRow][tempCol-3] == 0 && tempCol-4 >= 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol-4;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
								
							}else if(m[tempRow][tempCol-2] == 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol-2;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[1] > 1){ //se il muro non esce dalla scacchiera 
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la riga del nuovo muro verso sinistra
							tempPosWall[1] = tempPosWall[1]-2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick J_right pressed p1.28*/
		
		J_right++;
		switch(J_right){
			case 1:
				if(turnPlayer == 1 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer1[0];
						tempCol = currentPosPlayer1[1];
						if(spostato == 1 && (tempPosPlayer[1]+2 == currentPosPlayer1[1] || tempPosPlayer[1]+4 == currentPosPlayer1[1])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], White); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempCol+2 < 13 && m[tempRow][tempCol+1] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow][tempCol+2] == 1 && m[tempRow][tempCol+3] == 0 && tempCol+4 < 13){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol+4;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}else if(m[tempRow][tempCol+2] == 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol+2;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[1] < 11){ //se il muro non esce dalla scacchiera
							
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							
							//modifico la colonna del nuovo muro verso destra
							tempPosWall[1] = tempPosWall[1]+2;
							
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}else if(turnPlayer == 2 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer2[0];
						tempCol = currentPosPlayer2[1];
						if(spostato == 1 && (tempPosPlayer[1]+2 == currentPosPlayer2[1] || tempPosPlayer[1]+4 == currentPosPlayer2[1])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Red); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempCol+2 < 13 && m[tempRow][tempCol+1] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow][tempCol+2] == 1 && m[tempRow][tempCol+3] == 0 && tempCol+4 < 13){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol+4;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}else if(m[tempRow][tempCol+2] == 0){
								tempPosPlayer[0] = tempRow;
								tempPosPlayer[1] = tempCol+2;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[1] < 11){ //se il muro non esce dalla scacchiera 
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la riga del nuovo muro verso destra
							tempPosWall[1] = tempPosWall[1]+2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_right=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick J_up pressed p1.29*/
		
		J_up++;
		switch(J_up){
			case 1:
				if(turnPlayer == 1 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer1[0];
						tempCol = currentPosPlayer1[1];
						if(spostato == 1 && (tempPosPlayer[0]-2 == currentPosPlayer1[0] || tempPosPlayer[0]-4 == currentPosPlayer1[0])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], White); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempRow-2 >= 0 && m[tempRow-1][tempCol] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow-2][tempCol] == 1 && m[tempRow-3][tempCol] == 0 && tempRow-4 >= 0){
								tempPosPlayer[0] = tempRow-4;
								tempPosPlayer[1] = tempCol;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}else if(m[tempRow-2][tempCol] == 0){
								tempPosPlayer[0] = tempRow-2;
								tempPosPlayer[1] = tempCol;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer1[0], currentPosPlayer1[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[0] > 1){ //se il muro non esce dalla scacchiera
							
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la colonna del nuovo muro verso l'alto
							tempPosWall[0] = tempPosWall[0]-2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}else if(turnPlayer == 2 && enableJoystick == 1){
					if(modePlayer == 1){ //move player
						tempRow = currentPosPlayer2[0];
						tempCol = currentPosPlayer2[1];
						if(spostato == 1 && (tempPosPlayer[0]-2 == currentPosPlayer2[0] || tempPosPlayer[0]-4 == currentPosPlayer2[0])){ //ritorno a quella iniziale
							drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Red); //ridisegno il player nella posizione ideale
							drawSquares(tempPosPlayer[0], tempPosPlayer[1], Yellow);
							
							spostato = 0; //reset variabile
						}else if(tempRow-2 >= 0 && m[tempRow-1][tempCol] == 0 && spostato == 0){ //vado in una nuova casella
							if(m[tempRow-2][tempCol] == 1 && m[tempRow-3][tempCol] == 0 && tempRow-4 >= 0){
								tempPosPlayer[0] = tempRow-4;
								tempPosPlayer[1] = tempCol;
								//disegno il player
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}else if(m[tempRow-2][tempCol] == 0){
								tempPosPlayer[0] = tempRow-2;
								tempPosPlayer[1] = tempCol;
								//disegno il player nella nuova posizione
								drawPlayer(turnPlayer, tempPosPlayer[0], tempPosPlayer[1]);
								//oscuro il player dalla posizione precedente
								drawSquares(currentPosPlayer2[0], currentPosPlayer2[1], Black);
							}
							spostato = 1;
						}
					}else if(modePlayer == 2){ //wall player
						if(tempPosWall[0] > 1){ //se il muro non esce dalla scacchiera 
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							//modifico la riga del nuovo muro verso l'alto
							tempPosWall[0] = tempPosWall[0]-2;
							//disegno il nuovo muro
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_up=0;
	}
	
	
	/* button management */
	if(down_0 != 0){
		down_0++; 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */			
			switch(down_0){
				case 2:				/* pay attention here: please see slides to understand value 2 */
					operationInit();
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_0=0;			
			//NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_1!=0){ 
		down_1++;
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */			
			switch(down_1){
				case 2:				
					NVIC_EnableIRQ(EINT2_IRQn); //attivo pulsante KEY2

					if(turnPlayer == 1){
						if(wallsPlayer1 != 0){
							//se entra in modalità wall incremento di 1
							confirmWall++;
							//drawPlayer
							drawPlayer(turnPlayer, currentPosPlayer1[0], currentPosPlayer1[1]);
							//oscuro le possibili posizioni che potrebbe muovere
							drawBlackSquares(currentPosPlayer1[0], currentPosPlayer1[1]);
							if(confirmWall == 1){
								//inizializzo il muro centrale
								initializationWall();
								//setto la modalità del player a 2 (cioè sta muovendo un muro)
								modePlayer = 2;
							}else if(confirmWall == 2){ //se la mossa è valida
								
								if(positionWall == 2 && m[tempPosWall[0]][tempPosWall[1]] != 3 && m[tempPosWall[0]][tempPosWall[1]-1] != 3 && m[tempPosWall[0]][tempPosWall[1]+1] != 3  && checkWall() == 0){ //controllo se posso inserire il muro in modalità orizzontale
									GUI_Text(44+65, 270, (uint8_t *) "oo", Black, Black);
									//decremento la variabile dei muri del player 1
									wallsPlayer1--;
									//draw wall
									drawWall(tempPosWall[0], tempPosWall[1], positionWall, Blue2);
									//insert wall into vector
									insertWallMoves(tempPosWall[0], tempPosWall[1], turnPlayer, positionWall);
									//aggiorno il display
									PutChar(44, 280, '0'+wallsPlayer1, White, Black);
									timerMove = 0;
									NVIC_DisableIRQ(EINT2_IRQn); //disabilito il tasto KEY2
									break;
								}else if (positionWall == 1 && m[tempPosWall[0]][tempPosWall[1]] != 3 && m[tempPosWall[0]-1][tempPosWall[1]] != 3 && m[tempPosWall[0]+1][tempPosWall[1]] != 3  && checkWall() == 0){ //controllo se posso inserire il muro in modalità verticale
									GUI_Text(44+65, 270, (uint8_t *) "oo", Black, Black);
									wallsPlayer1--;
									//draw wall
									drawWall(tempPosWall[0], tempPosWall[1], positionWall, Blue2);
									//insert wall into vector
									insertWallMoves(tempPosWall[0], tempPosWall[1], turnPlayer, positionWall);
									//aggiorno il display
									PutChar(44, 280, '0'+wallsPlayer1, White, Black);
									timerMove = 0;
									NVIC_DisableIRQ(EINT2_IRQn); //disabilito il tasto KEY2
									break;
								}else{ //se non posso inserirlo
									confirmWall = 1;
									break;
								}
							}
						}else{
							confirmWall = 0;
						}
					}else{
						if(wallsPlayer2 != 0) {
							//se entra in modalità wall incremento di 1
							confirmWall++;
							//drawPlayer
							drawPlayer(turnPlayer, currentPosPlayer2[0], currentPosPlayer2[1]);
							//oscuro le possibili posizioni che potrebbe muovere
							drawBlackSquares(currentPosPlayer2[0], currentPosPlayer2[1]);
							 
							if(confirmWall == 1){
								//inizializzo il muro centrale
								initializationWall();
								//setto la modalità del player a 2 (cioè sta muovendo un muro)
								modePlayer = 2;
							}else if(confirmWall == 2){
								//controllo se posso inserirlo in modalità orizzontale
								if(positionWall == 2 && m[tempPosWall[0]][tempPosWall[1]] != 3 && m[tempPosWall[0]][tempPosWall[1]-1] != 3 && m[tempPosWall[0]][tempPosWall[1]+1] != 3 && checkWall() == 0){
									GUI_Text(44+65, 270, (uint8_t *) "oo", Black, Black);
									wallsPlayer2--;
									//draw wall of blue
									drawWall(tempPosWall[0], tempPosWall[1], positionWall, Blue2);
									//insert wall into vector
									insertWallMoves(tempPosWall[0], tempPosWall[1], turnPlayer, positionWall);
									//aggiorno display
									PutChar(44+2*65+2*8, 280, '0'+wallsPlayer2, White, Black);
									timerMove = 0;
									NVIC_DisableIRQ(EINT2_IRQn); //disabilito il tasto KEY2
									break;
								}else if (positionWall == 1 && m[tempPosWall[0]][tempPosWall[1]] != 3 && m[tempPosWall[0]-1][tempPosWall[1]] != 3 && m[tempPosWall[0]+1][tempPosWall[1]] != 3 && checkWall() == 0){ //controllo se posso inserire il muro in modalità verticale
									GUI_Text(44+65, 270, (uint8_t *) "oo", Black, Black);
									wallsPlayer2--;
									//draw wall of blue
									drawWall(tempPosWall[0], tempPosWall[1], positionWall, Blue2);
									//insert wall into vector
									insertWallMoves(tempPosWall[0], tempPosWall[1], turnPlayer, positionWall);
									//aggiorno display
									PutChar(44+2*65+2*8, 280, '0'+wallsPlayer2, White, Black);
									timerMove = 0;
									NVIC_DisableIRQ(EINT2_IRQn); //disabilito il tasto KEY2
									break;
								}else{ //se non posso inserirlo
									confirmWall = 1;
									break;
								}
							}
							
						}else{
							confirmWall = 0;
						}
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_2 != 0){
		down_2++;	
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */			
			switch(down_2){
				case 2:		
						if(positionWall == 1){ //if wall is vertical
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							
							positionWall = 2; //change from vertical to horizontal
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}else if(positionWall == 2){ //if wall is horizontal
							//oscuro il muro precedente
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Black);
							
							positionWall = 1; //change horizontal to vertical
							drawWall(tempPosWall[0], tempPosWall[1], positionWall, Yellow);
						}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
		
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
