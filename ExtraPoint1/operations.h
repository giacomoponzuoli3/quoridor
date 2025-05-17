#include "LPC17xx.h"



/* Private function prototypes -----------------------------------------------*/
void insertPlayerMoves(uint16_t row, uint16_t col, int numPlayer);
void insertWallMoves(uint16_t row, uint16_t col, int numPlayer, int posWall);
void drawSecondsOnDislay(void);
void drawPossibilyMoves(int numPlayer);
void drawPlayer(int numPlayer, int row, int col);
void drawSquares(int row, int col, uint16_t color);
void drawBlackSquares(int row, int col);
void drawWall(int rowCenter, int colCenter, int position, uint16_t color); //position: verticale o orizzontale
void initializationWall(void);
void operationInit(void);
void dfs(int row, int col, int visited[13][13]);
int checkWall(void);

