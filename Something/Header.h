#pragma once
#include <vector>
#define WHITE 'W'
#define BLACK 'B'
const int width = 8;
const int height = 8;

void PrintBoard();
void CreateBoard();
void MakeMove(int, int, int, int);
void Move();
void StartGame();
bool IsCheck(char);
bool AnyValidMoves(char);
char REPICKPIECE = 'N';
bool GameOver();
void NextPlayer();
char player = WHITE;
bool playingVersusAI = true;
std::string AIMove(char);
int minimax(char);
std::vector<std::string> previousMoves;
class Piece {
public:
	Piece(char newColor):myColor(newColor){}
	virtual char GetPiece() = 0;
	char GetColor() {
		return myColor;
	}
	bool LegalMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//Check if there is a piece there, if there is check if its the opposite of myColor
		Piece* destPiece = board[destHeight][destWidth];
		if ((destPiece == 0) || (myColor != destPiece->GetColor())) {
			return ValidMove(board, destHeight, destWidth, myHeight, myWidth);
		}
		else {
			return false;
		}
	}
private:
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) = 0;
	char myColor;
};

Piece* pieces[height][width];

class Pawn :public Piece {
public:
	Pawn(char newColor) :Piece(newColor) {}
	virtual char GetPiece() {
		return 'P';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//Check if the target destination is a NULL(no piece there), if so check only +/-(depening on color)1 on height
		//Black should me +1, white -1
		Piece* dest = board[destHeight][destWidth];
		int direction = 0;
		if (GetColor() == WHITE) {
			direction = 1;
		}
		else if(GetColor() == BLACK){
			direction = -1;
		}
		if (dest == 0) {
			if (destHeight + direction == myHeight && destWidth == myWidth) {
				return true;
			}
		}
		else if (dest != 0) {
			//Check if its a +/-1 height, and +/-1 width, then take it
			if (destHeight + direction == myHeight && (destWidth + 1 == myWidth || destWidth - 1 == myWidth)) {
				return true;
			}
		}
		return false;
	}
};
class Bishop:public Piece {
public:
	Bishop(char newColor) :Piece(newColor) {}
private:
	//Note to later, check diagonal moves, for each up, for each sideways, check that up-sideways > 0 and otherway around  if sideways > up
	virtual char GetPiece() {
		return 'B';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//Can only move diagonaly so destHeight and destWidth has to be == 
		if ((destWidth - myWidth == destHeight - myHeight) || (destWidth - myWidth == myHeight - destHeight)) {
			int xDir = (destHeight - myHeight > 0) ? 1 : -1;
			int yDir = (destWidth - myWidth > 0) ? 1 : -1;
			int k;
			int i;
			for (i = myHeight + xDir, k = myWidth + yDir; i != destHeight; i = i + xDir, k = k + yDir)
			{
				if (board[i][k] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};
class Knight:public Piece {
public:
	Knight(char newColor):Piece(newColor){}
private:
	virtual char GetPiece() {
		return 'k';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//Either destWidth or destHeight has to be 1/-1, then the other has to be one has to be +/-3
		if (myHeight == destHeight + 1 || myHeight == destHeight - 1) {
			if (myWidth == destWidth + 2 || myWidth == destWidth - 2) {
				return true;
			}
		}
		else if (myHeight == destHeight + 2 || myHeight == destHeight - 2) {
			if (myWidth == destWidth + 1 || myWidth == destWidth - 1) {
				return true;
			}
		}
		return false;
	}
};
class Rook:public Piece {
public:
	Rook(char newColor):Piece(newColor){}
private:
	virtual char GetPiece() {
		return 'R';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//if only move sideways
		if (myHeight == destHeight) {
			//Find what direction its moving
			int dir = 0;
			if (myWidth - destWidth > 0) {
				dir = 1;
			}
			else {
				dir = -1;
			}
			//myWidth + dir in init to not check its own place, wich would return false
			for (int pos = myWidth + dir; pos != destWidth; pos = pos + dir) {
				if (board[myHeight][pos] != 0) {
					return false;
				}
			}
			return true;
		}
		//If only move vertical
		if (myWidth == destWidth) {
			int dir = 0;
			if (myHeight - destHeight > 0) {
				dir = 1;
			}
			else {
				dir = -1;
			}
			for (int pos = myHeight + dir; pos != destHeight; pos = pos + dir) {
				if (board[myHeight][pos] != 0) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};
class Queen:public Piece {
public:
	Queen(char newColor) :Piece(newColor) {}
private:
	virtual char GetPiece() {
		return 'Q';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		//if only move sideways
		if (myHeight == destHeight) {
			//Find what direction its moving
			int dir = 0;
			if (myWidth - destWidth > 0) {
				dir = 1;
			}
			else {
				dir = -1;
			}
			//myWidth + dir in init to not check its own place, wich would return false
			for (int pos = myWidth + dir; pos != destWidth; pos = pos + dir) {
				if (board[myHeight][pos] != 0) {
					return false;
				}
			}
			return true;
		}
		//If only move vertical
		if (myWidth == destWidth) {
			int dir = 0;
			if (myHeight - destHeight > 0) {
				dir = 1;
			}
			else {
				dir = -1;
			}
			for (int pos = myHeight + dir; pos != destHeight; pos = pos + dir) {
				if (board[myHeight][pos] != 0) {
					return false;
				}
			}
			return true;
		}
		//Check if it moves diagonally
		if ((destWidth - myHeight == destHeight - myHeight) || (destWidth - myHeight == myHeight - destHeight)) {
			int xDir = (destHeight - myHeight > 0) ? 1 : -1;
			int yDir = (destWidth - myWidth > 0) ? 1 : -1;
			int k = 0;
			for (int i = myHeight + xDir, k = myWidth + yDir; i != destHeight; i = i + xDir, k = k + yDir)
			{
				if (board[i][k] != 0) {
					return false;
				}
			}
		}

		return true;
	}
};
class King:public Piece {
public:
	King(char newColor) :Piece(newColor) {}
private:
	virtual char GetPiece() {
		return 'K';
	}
	virtual bool ValidMove(Piece* board[height][width], int destHeight, int destWidth, int myHeight, int myWidth) {
		int deltaX = destHeight - myHeight;
		int deltaY = destWidth - myWidth;
		if ((deltaX >= -1 && deltaX <= 1) && (deltaY >= -1 && deltaY <= 1)) {
			return true;
		}
		else {
			return false;
		}
	}
};




