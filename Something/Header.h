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
		if (destHeight - myHeight == destWidth - myWidth || destHeight - myHeight == myWidth - destWidth) {
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
		return 'K';
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
		if (myHeight == destHeight || myWidth == destWidth) {
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
		if (myHeight == destHeight || myWidth == destWidth) {
			return true;
		}
		else if (destHeight == destWidth) {
			return true;
		}
		return false;
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
		if (myHeight == destHeight - 1 || myHeight == destHeight + 1 || myHeight == destHeight) {
			if (myWidth == destWidth - 1 || myWidth == destWidth + 1 || myWidth == destWidth) {
				return true;
			}
		}
		return true;
	}
};

Piece* pieces[height][width];

std::vector<std::string> previousMoves;

