#include "operations.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/AsciiLib.h"

#include <stdio.h>
#include <math.h>

/*******************************************************************************
Global variables
*******************************************************************************/

extern int wallsPlayer1;
extern int wallsPlayer2;
extern int timerMove;
extern int const delta;
extern int const spaceSquare;
extern uint32_t moves[200];
extern int i;
extern int m[13][13];
extern int turnPlayer;
extern int currentPosPlayer1[2];
extern int currentPosPlayer2[2];
extern int tempPosWall[2];
extern int positionWall;
extern int modePlayer;
extern int confirmWall;
extern int enableJoystick;
extern int visited[13][13];
extern int confirmMove;
extern int spostato;
extern int confirmMove;


/******************************************************************************
* Function Name : insertPlayerMoves
*******************************************************************************/

void insertPlayerMoves(uint16_t row, uint16_t col, int numPlayer){
	if (numPlayer == 1){
		if(confirmMove == 1){
			//insert moves 
			moves[i++] = 0 | (0 << 24) //insert num player
										| (0 << 20) //insert PlayerMove
										| (0 << 16) //Vertical/Horizontal
										| (row << 8) //insert num row
										| (col << 0); //insert num col
			m[row][col] = 1;
			
			//save the current position
			currentPosPlayer1[0] = row;
			currentPosPlayer1[1] = col;
		}else{
			//inserimento scaduto il timer
			moves[i++] = 0 | (0 << 24) //insert num player
										| (0 << 20) //insert PlayerMove
										| (1 << 16) //Vertical/Horizontal
										| (row << 8) //insert num row
										| (col << 0); //insert num col
			//save the current position
			currentPosPlayer1[0] = row;
			currentPosPlayer1[1] = col;
		}
	}else if (numPlayer == 2){
		if(confirmMove == 1){
			//insert moves
			moves[i++] = 0 | (1 << 24) //insert num player
										| (0 << 20) //insert PlayerMove
										| (0 << 16) //Vertical/Horizontal
										| (row << 8) //insert num row
										| (col << 0); //insert num col
			m[row][col] = 1;
			//save the current position
			currentPosPlayer2[0] = row;
			currentPosPlayer2[1] = col;
		}else{
			//insert moves
			moves[i++] = 0 | (1 << 24) //insert num player
										| (0 << 20) //insert PlayerMove
										| (1 << 16) //Vertical/Horizontal
										| (row << 8) //insert num row
										| (col << 0); //insert num col
			//save the current position
			currentPosPlayer2[0] = row;
			currentPosPlayer2[1] = col;
		}
	}
	
}


/******************************************************************************
* Function Name : insertWallMoves
* posWall :
* - 1: vertical
* - 2: horizontal
*******************************************************************************/
void insertWallMoves(uint16_t row, uint16_t col, int numPlayer, int posWall){
	int tempPosWall = posWall - 1;
	int tempNumPlayer = numPlayer -1;
	int tempFactorX, tempFactorY, index, xTemp, yTemp;
	//insert wall
	moves[i++] = 0 | (tempNumPlayer << 24) //insert num player
								| (1 << 20) //insert wall move
								| (tempPosWall << 16) //Vertical/Horizontal
								| (row << 8) //insert num row of center of wall
								| (col << 0); //insert num col of center of wall
		
	//aggiorno matrice
	if(posWall == 1){ //vertical
		m[row-1][col] = 3;
		m[row][col] = 3;
		m[row+1][col] = 3;
		if(row-3 > 1 && m[row-3][col] == 3){
			m[row-2][col] = 3;
			//colorare solo il pezzo centrale di dim 5x5
			tempFactorX = (col-1) / 2;
			tempFactorY = (row-3) / 2;
			xTemp = 15 + (tempFactorX+1)*delta + tempFactorX*spaceSquare;
			yTemp = 15 + (tempFactorY+1)*delta + tempFactorY*spaceSquare + 1;
			
			for(index = 0; index<spaceSquare; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare, yTemp, Blue2);
				yTemp = yTemp + 1;
			}
		}
		if(row+3 < 11 && m[row+3][col] == 3){
			m[row+2][col] = 3;
			//colorare solo il pezzo centrale di dim 5x5
			tempFactorX = (col-1) / 2;
			tempFactorY = (row+1) / 2;
			xTemp = 15 + (tempFactorX+1)*delta + tempFactorX*spaceSquare;
			yTemp = 15 + (tempFactorY+1)*delta + tempFactorY*spaceSquare + 1;
			
			for(index = 0; index<spaceSquare; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare, yTemp, Blue2);
				yTemp = yTemp + 1;
			}
		}
	}else if(posWall == 2){ //horizontal
		m[row][col-1] = 3;
		m[row][col] = 3;
		m[row][col+1] = 3;
		
		if(col-3 > 1 && m[row][col-3] == 3){
			m[row][col-2] = 3;
			tempFactorX = (col-3) / 2;
			tempFactorY = (row-1) / 2;
			xTemp = 15 + (tempFactorX+1)*delta + tempFactorX*spaceSquare;
			yTemp = 15 + (tempFactorY+1)*delta + tempFactorY*spaceSquare + 1;
			//colorare solo il pezzo centrale di dim 5x5
			for(index = 0; index<spaceSquare; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare, yTemp, Blue2);
				yTemp = yTemp + 1;
			}
		}
		if(col+3 < 11 && m[row][col+3] == 3){
			m[row][col+2] = 3;
			//colorare solo il pezzo centrale di dim 5x5
			tempFactorX = (col+1) / 2;
			tempFactorY = (row-1) / 2;
			xTemp = 15 + (tempFactorX+1)*delta + tempFactorX*spaceSquare;
			yTemp = 15 + (tempFactorY+1)*delta + tempFactorY*spaceSquare + 1;
			
			for(index = 0; index<spaceSquare; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare, yTemp, Blue2);
				yTemp = yTemp + 1;
			}
			
		}
		
	}
}


/******************************************************************************
* Function Name : drawSecondsOnDislay
*******************************************************************************/
static int i1;
void drawSecondsOnDislay(void){
	char temp[3];
	if(timerMove == 0){
		sprintf(temp, "%d", timerMove);
		GUI_Text(44+65+8, 270, (uint8_t *) temp, White, Black);
	}else{
		if(timerMove >= 10){
			sprintf(temp, "%d", timerMove);
			GUI_Text(44+65, 270, (uint8_t *) temp, White, Black);
		}else{
			for(i1 = 0; i1 < 30; i1++){
				LCD_DrawLine(44+65, 270+i1, 44+65+4, 270+i1, Black);
			}
			PutChar(44+65+8, 270, '0'+timerMove, White, Black);
		}
	}
}


/******************************************************************************
* Function Name : drawPossibilyMoves
*******************************************************************************/

void drawPossibilyMoves(int numPlayer){
	int row, col;
			
	if(numPlayer == 1){
		row = currentPosPlayer1[0];
		col = currentPosPlayer1[1];
	}else if(numPlayer == 2){
		row = currentPosPlayer2[0];
		col = currentPosPlayer2[1];
	}
	
	if(row+2<13 && m[row+1][col] == 0){ //colorare quadrato a in basso
		if(m[row+2][col] == 1 && m[row+3][col] == 0 && row+4<13) //vuol dire che c'è l'altro player
			drawSquares(row+4, col, Yellow);
		else if(m[row+2][col] == 0)
			drawSquares(row+2, col, Yellow); 
	}
	if(row-2>=0 && m[row-1][col] == 0){ //colorare quadrato a in alto
		if(m[row-2][col] == 1 && m[row-3][col] == 0 && row-4 >= 0) //vuol dire che c'è l'altro player
			drawSquares(row-4, col, Yellow);
		else if(m[row-2][col] == 0)
			drawSquares(row-2, col, Yellow); 
	}
	if(col+2<13 && m[row][col+1] == 0){ //colorare quadrato a destra
		if(m[row][col+2] == 1 && m[row][col+3] == 0 && col+4 < 13) //vuol dire che c'è l'altro player
			drawSquares(row, col+4, Yellow);
		else if(m[row][col+2] == 0)
			drawSquares(row, col+2, Yellow); 
	}
	if(col-2>=0 && m[row][col-1] == 0){ //colorare quadrato a sinistra
		if(m[row][col-2] == 1 && m[row][col-3] == 0 && col-4 >= 0) //vuol dire che c'è l'altro player
			drawSquares(row, col-4, Yellow);
		else if(m[row][col-2] == 0)
			drawSquares(row, col-2, Yellow); 
	}
}


/******************************************************************************
* Function Name : drawPlayer
*******************************************************************************/

void drawPlayer(int numPlayer, int row, int col){
	//row = Y
	//col = X

	if(numPlayer == 1)	
		drawSquares(row, col, White);
	else if(numPlayer == 2)
		drawSquares(row, col, Red);
	
	//insert moves se il timer è pari a 0
	if(spostato == 1)
		insertPlayerMoves(row, col, numPlayer);

}



/******************************************************************************
* Function Name : drawSquares
*******************************************************************************/

void drawSquares(int row, int col, uint16_t color){
	
	int x = 15;
	int y = 15;
	int z;
	
	int xTemp, yTemp, factor;
	
	if(row !=0 && row % 2 == 0){
		factor = row/2;
		y = y+factor*delta+factor*spaceSquare; 
	}
	if(col != 0 && col % 2 == 0){
		factor = col/2;
		x = x+factor*delta+factor*spaceSquare;
	}
	
	xTemp = x+1;
	yTemp = y+1;
	
	for(z = 0; z < 24; z++){
		LCD_DrawLine(xTemp, yTemp, xTemp+delta-2, yTemp, color); //lato alto
		yTemp = yTemp + 1;
	}
	
}


/******************************************************************************
* Function Name : drawBlackSquares
*******************************************************************************/
void drawBlackSquares(int row, int col){

	if(row+2<13){ //colorare quadrato a in basso 
		if(m[row+2][col] == 0)
			drawSquares(row+2, col, Black); 
		else if(row+4 < 13 && m[row+4][col] == 0)
			drawSquares(row+4, col, Black);	
	}
	if(row-2>=0){ //colorare quadrato a in alto
		if(m[row-2][col] == 0)
			drawSquares(row-2, col, Black); 
		else if(row-4 >= 0 && m[row-4][col] == 0)
			drawSquares(row-4, col, Black);
	}
	if(col+2<13){ //colorare quadrato a destra
		if(m[row][col+2] == 0)
			drawSquares(row, col+2, Black); 
		else if(col+4 < 13 && m[row][col+4] == 0)
			drawSquares(row, col+4, Black);
	}
	if(col-2>=0){
		if(m[row][col-2] == 0)
			drawSquares(row, col-2, Black);
		else if(col-4 >= 0 && m[row][col-4] == 0)
			drawSquares(row, col-4, Black);
	}
}



/******************************************************************************
* Function Name : drawWall
* disegna il muro in base alla posizione (verticale/orizzontale)
* position = 0 => muro orizzontale
* position = 1 => muro verticale
*******************************************************************************/
void drawWall(int rowCenter, int colCenter, int position, uint16_t color){
	int xTemp, yTemp, index, factorX, factorY;
	
	factorX = (colCenter-1) / 2;
	factorY = (rowCenter-1) / 2;
	//occupa 3 (2+1) spazi della matrice 
	if (position == 2){ //orizzontale
		if(color == Black){
			if(m[rowCenter][colCenter-1] == 0 && m[rowCenter][colCenter+1] == 0 && m[rowCenter][colCenter] == 0){ //se non è presente un muro nella posizione 
				xTemp = 15 + factorX*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
				for(index = 0; index<spaceSquare-1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+2*delta+spaceSquare, yTemp, color);
					yTemp = yTemp+1;
				}
			}else if(m[rowCenter][colCenter-1] == 3 && m[rowCenter][colCenter+1] == 3 && m[rowCenter][colCenter] == 3){
				xTemp = 15 + factorX*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
				for(index = 0; index<spaceSquare-1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+2*delta+spaceSquare, yTemp, Blue2);
					yTemp = yTemp+1;
				}
			}else if(m[rowCenter][colCenter] == 3 && m[rowCenter][colCenter+1] == 0 && m[rowCenter][colCenter-1] == 0){ //se il muro era presente solo nella parte centrale
				xTemp = 15 + factorX*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
				for(index = 0; index<spaceSquare-1; index++){ 
					LCD_DrawLine(xTemp, yTemp, xTemp+delta, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				xTemp = xTemp + delta + spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				for(index = 0; index<spaceSquare-1; index++){ 
					LCD_DrawLine(xTemp, yTemp, xTemp+delta, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro di blu la parte centrale
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				for(index = 0; index<spaceSquare-1; index++){ 
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare, yTemp, Blue2);
					yTemp = yTemp + 1;
				}
				
				
			}else if(m[rowCenter][colCenter-1] == 3 && m[rowCenter][colCenter-2] == 3){ //se il muro era presente solo nella parte sinstra
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
				for(index = 0; index<spaceSquare-1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+delta+spaceSquare, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro di blue la parte destra del muro già presente
				xTemp = 15 + factorX*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				for(index = 0; index<spaceSquare-1; index++){ 
					LCD_DrawLine(xTemp, yTemp, xTemp+delta, yTemp, Blue2);
					yTemp = yTemp + 1;
				}
				
			}else if(m[rowCenter][colCenter+1] == 3 && m[rowCenter][colCenter+2] == 3){ //se il muro era presente solo nella parte destra
				xTemp = 15 + factorX*delta + factorX*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
				for(index = 0; index<spaceSquare-1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+delta+spaceSquare, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro di blue la parte sinistra del muro già presente
				xTemp = 15 + (factorX+1)*delta + (factorX+1)*spaceSquare;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare+1;
				for(index = 0; index<spaceSquare-1; index++){ 
					LCD_DrawLine(xTemp, yTemp, xTemp+delta, yTemp, Blue2);
					yTemp = yTemp + 1;
				}
				
			}
		}else{ //se il muro è giallo o blu
			xTemp = 15 + factorX*delta + factorX*spaceSquare;
			yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare + 1;
				
			for(index = 0; index<spaceSquare-1; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+2*delta+spaceSquare, yTemp, color);
				yTemp = yTemp+1;
			}
		}
		
	}else if(position == 1){ //verticale
		if(color == Black){
			if(m[rowCenter-1][colCenter] == 0 && m[rowCenter+1][colCenter] == 0 && m[rowCenter][colCenter] == 0){ //se non è presente un muro nella posizione
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + factorY * delta + factorY *spaceSquare;
				
				for(index = 0; index<2*delta+spaceSquare+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
					yTemp = yTemp+1;
				}

			}else if(m[rowCenter-1][colCenter] == 3 && m[rowCenter+1][colCenter] == 3 && m[rowCenter][colCenter] == 3){ //se è gia presente un muro in quella posizione
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + factorY * delta + factorY *spaceSquare;
				
				for(index = 0; index<2*delta+spaceSquare+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, Blue2);
					yTemp = yTemp+1;
				}
			}else if(m[rowCenter][colCenter] == 3 && m[rowCenter+1][colCenter] == 0 && m[rowCenter-1][colCenter] == 0){ //se il muro era presente solo nella parte centrale
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + factorY*delta + factorY*spaceSquare;
				
				for(index = 0; index<delta+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
					yTemp = yTemp + 1;
				}				
				
				yTemp = 15 + (factorY+1)*delta + (factorY+1)*spaceSquare;
				for(index = 0; index<delta+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro la parte centrale
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare;
				for(index = 0; index<spaceSquare-1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, Blue2);
					yTemp = yTemp + 1;
				}
				
			}else if(m[rowCenter-1][colCenter] == 3 && m[rowCenter-2][colCenter] == 3){ //se è gia presente un muro nella parte alta 
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + (factorY+1)*delta + factorY*spaceSquare;
				
				for(index = 0; index<delta+spaceSquare+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro la parte bassa del muro già presente
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + factorY*delta + factorY*spaceSquare;
				for(index = 0; index<delta+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, Blue2);
					yTemp = yTemp + 1;
				}
					
				
			}else if(m[rowCenter+1][colCenter] == 3 && m[rowCenter+2][colCenter] == 3){ //se è presente un muro nella parte sotto
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + factorY*delta + factorY*spaceSquare;
				
				for(index = 0; index<delta+spaceSquare+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
					yTemp = yTemp + 1;
				}
				
				//ricoloro la parte in alto del muro già presente
				xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
				yTemp = 15 + (factorY+1)*delta + (factorY+1)*spaceSquare;
				for(index = 0; index<delta+1; index++){
					LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, Blue2);
					yTemp = yTemp + 1;
				}	
				
			}
		}else{
			xTemp = 15 + (factorX+1)*delta + factorX*spaceSquare + 1;
			yTemp = 15 + factorY * delta + factorY *spaceSquare;
				
			for(index = 0; index<2*delta+spaceSquare+1; index++){
				LCD_DrawLine(xTemp, yTemp, xTemp+spaceSquare-2, yTemp, color);
				yTemp = yTemp+1;
			}
		}
	}
}


/******************************************************************************
* Function Name : initializationWall
*******************************************************************************/
void initializationWall(void){
	positionWall = 2;
	drawWall(5, 7, positionWall, Yellow);
	tempPosWall[0] = 5;
	tempPosWall[1] = 7;
}



/********************************************************
* Function Name : operationInit()
*
* Introdotta perché non mi funziona il tasto KEY0 con emulatore
*********************************************************/
void operationInit(void){
	NVIC_DisableIRQ(EINT0_IRQn); //disattivo l'interruttore KEY0
	turnPlayer = 1; 
	modePlayer = 1; //mode move player
	confirmMove = 0; //set confirmMove a 0 
	confirmWall = 0;
	
	//coloro la linea per far capire a quale giocatore sta (P1)
	LCD_DrawLine(12, 250, 12, 250+50, Yellow); //linea verticale sinistra
	LCD_DrawLine(12, 250, 12+65, 250, Yellow); //linea alta
	LCD_DrawLine(12+65, 250, 12+65, 250+50, Yellow); //linea laterale destra
	LCD_DrawLine(12, 250+50, 12+65, 250+50, Yellow); //linea bassa
	
	drawPossibilyMoves(turnPlayer);
	
	init_timer(0, 0x017D7840 ); 						    /* 1s */
	enable_timer(0);
	
	NVIC_EnableIRQ(EINT1_IRQn); //attivo l'interruttore KEY1 per entrare in modalità muro
	
	enableJoystick = 1; //attivo l'utilizzo del joystick
}


/**********************************************************
* Function Name : dfs
* rowEnemy : riga 
* colEnemy : colonna 
* visited[13][13] : matrice che permette di informare quali sono le celle che possono essere visitate
***********************************************************/
void dfs(int row, int col, int visited[13][13]){
	if(row < 0 || col < 0 || row > 12 || col > 12 || m[row][col] == 3 || visited[row][col] == 1)
		return;
	visited[row][col] = 1;
	
	dfs(row-1, col, visited);
	dfs(row+1, col, visited);
	dfs(row, col-1, visited);
	dfs(row, col+1, visited);
	
}



/**********************************************************
* Function Name : checkWall
* funzione che mi permette di identificare che posso inserire il muro o no
* return 0 : mossa valida
* return 1 : mossa non valida
***********************************************************/
int checkWall(void){
	int rowIndex, colIndex, indexVisited;
	
	//inizializzo la matrice visited
	for(rowIndex = 0; rowIndex < 13; rowIndex++)
		for(colIndex = 0; colIndex < 13; colIndex++)
			visited[rowIndex][colIndex] = 0;
	
	//inserisco "anticipatamente" il muro per controllare se può essere inserito o meno
	m[tempPosWall[0]][tempPosWall[1]] = 3;
	if(positionWall == 1){ //verticale
		m[tempPosWall[0]+1][tempPosWall[1]] = 3;
		m[tempPosWall[0]-1][tempPosWall[1]] = 3;
	}else if(positionWall == 2){ //orizzontale
		m[tempPosWall[0]][tempPosWall[1]+1] = 3;
		m[tempPosWall[0]][tempPosWall[1]-1] = 3;
	}
	
	if(turnPlayer == 1){ //se gioca il player 0
											//devo passa la posizione del player 1
		dfs(currentPosPlayer2[0], currentPosPlayer2[1], visited);
		
		//resetto la matrice m
		m[tempPosWall[0]][tempPosWall[1]] = 0;
		if(positionWall == 1){ //verticale
			m[tempPosWall[0]+1][tempPosWall[1]] = 0;
			m[tempPosWall[0]-1][tempPosWall[1]] = 0;
		}else if(positionWall == 2){ //orizzontale
			m[tempPosWall[0]][tempPosWall[1]+1] = 0;
			m[tempPosWall[0]][tempPosWall[1]-1] = 0;
		}
		
		//controllo se esiste almeno una cella nella riga 0 in cui è possibilw visitarla in modo da non bloccare il player 1
		for(indexVisited = 0; indexVisited < 13; indexVisited = indexVisited + 2)
			if(visited[0][indexVisited] == 1)
				return 0; 
		
	}else if(turnPlayer == 2){ //se gioca il player 1
											//devo passa la posizione del player 0
		dfs(currentPosPlayer1[0], currentPosPlayer1[1], visited);
		
		//resetto la matrice m
		m[tempPosWall[0]][tempPosWall[1]] = 0;
		if(positionWall == 1){ //verticale
			m[tempPosWall[0]+1][tempPosWall[1]] = 0;
			m[tempPosWall[0]-1][tempPosWall[1]] = 0;
		}else if(positionWall == 2){ //orizzontale
			m[tempPosWall[0]][tempPosWall[1]+1] = 0;
			m[tempPosWall[0]][tempPosWall[1]-1] = 0;
		}
		
		//controllo se esiste almeno una cella nella riga 12 in cui è possibilw visitarla in modo da non bloccare il player 0
		for(indexVisited = 0; indexVisited < 13; indexVisited = indexVisited + 2)
			if(visited[12][indexVisited] == 1)
				return 0; 
	}
	
	//se arriva qui vuol dire che la mossa non è valida quindi resettiamo la matrice m
	m[tempPosWall[0]][tempPosWall[1]] = 0;
	if(positionWall == 1){ //verticale
		m[tempPosWall[0]+1][tempPosWall[1]] = 0;
		m[tempPosWall[0]-1][tempPosWall[1]] = 0;
	}else if(positionWall == 2){ //orizzontale
		m[tempPosWall[0]][tempPosWall[1]+1] = 0;
		m[tempPosWall[0]][tempPosWall[1]-1] = 0;
	}
	
	return 1; //mossa non valida
}



