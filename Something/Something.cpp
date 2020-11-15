#include <iostream>
#include <string>
#include "Header.h"

int main()
{
    CreateBoard();
    PrintBoard();
    Move();
    return 0;
}

void CreateBoard() {
    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++)
        {
            char color;
            if (i == 0 || i == 1) {
                color = BLACK;
            }
            else {
                color = WHITE;
            }
            pieces[i][k] = NULL;
            //Rooks
            if ((i == 0 || i == height - 1) && (k == 0 || k == width - 1)) {
                pieces[i][k] = new Rook(color);
            }
            //Knights
            if ((i == 0 || i == height - 1) && (k == 1 || k == width - 2)) {
                pieces[i][k] = new Knight(color);
            }
            //Bishops
            if ((i == 0 || i == height - 1) && (k == 2 || k == width - 3)) {
                pieces[i][k] = new Bishop(color);
            }
            //Queen
            if ((i == 0 && k == 3) || (i == height-1 && k == 4)) {
                pieces[i][k] = new Queen(color);
            }
            if ((i == 0 && k ==4) || (i == height - 1 && k == 3)) {
                pieces[i][k] = new King(color);
            }
            //Second and second last rows are pawns
            if (i == 1 || i == height - 2) {
                pieces[i][k] = new Pawn(color);
            }
        }
    }
}

void PrintBoard() {
    for (int i = 0; i < height; i++) {
        std::cout << i <<"|";
        for (int k = 0; k < width; k++)
        {
            Piece* a = pieces[i][k];
            if (a != 0) {
                std::cout << a->GetPiece() << "|";
            }
            else {
                std::cout << " |";
            }
        }
        std::cout << "\n";
    }
    std::cout << " ";
    for (int k = 0; k < width; k++)
    {
        std::cout << " " << k;
    }
}

void Move() {
    //Check for valid pawn moves
    //Ask what pawn to move
    while (true) {
        system("cls");
        PrintBoard();

        std::string input;
        std::cout << "\nWhat pawn to move?\n";

        getline(std::cin, input);

        int x = input[0] - '0';
        int y = input[1] - '0';

        Piece* pieceToMove = pieces[x][y];
        std::cout << "This is a " << pieceToMove->GetPiece() << "Which is a " << pieceToMove->GetColor() << "\n";

        std::cout << "Where to move it?\n";
        getline(std::cin, input);
        int t_x = input[0] - '0';
        int t_y = input[1] - '0';

        std::cout << "\nYou wanted to move to: " << t_x << t_y;
        bool a = pieceToMove->LegalMove(pieces, t_x, t_y, x, y);
        if (a == true) {
            pieces[x][y] = 0;
            pieces[t_x][t_y] = pieceToMove;
            std::cout << "\nThis move is legal\n";
            PrintBoard();
        }
        else {
            std::cout << "\nThis move is not legal";
        }
    }
    
    
}