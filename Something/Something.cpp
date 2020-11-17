#include <iostream>
#include <string>
#include "Header.h"

int main()
{
    StartGame();
    return 0;
}

void StartGame() {
    player = WHITE;
    CreateBoard();
    while (!GameOver()) {
        Move();
        NextPlayer();
    }
    Move();
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

        if (i == 0) {
            for (int j = 0; j < previousMoves.size(); j++)
            {
                if (j == 0) {
                    std::cout << "\t";
                }
                std::cout << previousMoves[j] << ", ";
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
    /*while (true) {
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
            pieces[x][y] = NULL;
            pieces[t_x][t_y] = pieceToMove;
            MakeMove(x, y, t_x, t_y);
            std::cout << "\nThis move is legal\n";
            PrintBoard();
        }
        else {
            std::cout << "\nThis move is not legal";
            system("pause");
        }
    }*/

    std::string input;
    Piece* pieceToMove = NULL;
    Piece* temp = NULL;
    int state = 0;
    int x = 0;
    int y = 0;
    int t_x = 0;
    int t_y = 0;
    while (true) {
        switch (state)
        {
        case 0:
            system("cls");
            PrintBoard();
            //Ask for what piece to move
            std::cout << "\nWhat pawn to move?\n";

            getline(std::cin, input);

            x = input[0] - '0';
            y = input[1] - '0';

            //Check if its a valid piece
            if (pieces[x][y] == NULL) {
                std::cout << "This is not a piece!\n";
                system("pause");
                break;
            }
            //Check if the piece is the same as the current turn
            if (pieces[x][y]->GetColor() != player) {
                std::cout << "This is your opponents piece!";
                system("pause");
                break;
            }
            pieceToMove = pieces[x][y];
            state = 1;
            break;
        case 1:
            system("cls");
            PrintBoard();
            //Instructions
            std::cout << "\n[N] to pick another piece.";
            std::cout << "\nMoving: " << pieceToMove->GetPiece() << "[" << std::to_string(x) << ", " << std::to_string(y) << "]";
            //Ask for where to move
            std::cout << "\nWhere to move it?\n";
            getline(std::cin, input);
            //Check if the player wants to pick another piece instead
            if (toupper(input[0]) == REPICKPIECE) {
                //Back to piece picker
                state = 0;
                break;
            }
            //Check if its a valid move
            t_x = input[0] - '0';
            t_y = input[1] - '0';
            if (!pieces[x][y]->LegalMove(pieces, t_x, t_y, x, y)) {
                std::cout << "This is an illegal move!";
                break;
            }
            //Move it and check the move dont set you i check
            temp = pieces[x][y];
            pieces[x][y] = 0;
            pieces[t_x][t_y] = pieceToMove;
            if (IsCheck(player)) {
                //Undo move and give error message
                pieces[x][y] = pieceToMove;
                pieces[t_x][t_y] = temp;
                std::cout << "This move would set you in check, illegal!";
                system("pause");
                break;
            }
            MakeMove(x, y, t_x, t_y);
            state = 0;
            //If it gets to this state, return the whole function
            return;
        default:
            break;
        }
    }
    
    
}

void MakeMove(int fromX, int fromY, int toX, int toY) {
    std::string temp = "[" + std::to_string(fromX) + "," + std::to_string(fromY) + "] to ["+ std::to_string(toX) + "," + std::to_string(toY) +"]";
    previousMoves.push_back(temp);
}

bool IsCheck(char color) {
    //Get king x and y
    int kingX;
    int kingY;

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            if (pieces[i][k] != 0) {
                if (pieces[i][k]->GetColor() == color) {
                    if (pieces[i][k]->GetPiece() == 'K') {
                        kingX = i;
                        kingY = k;
                    }
                }
            }
        }
    }
    //Check every !=color piece if the king x and y is a valid move
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            if (pieces[i][k] != 0) {
                if (pieces[i][k]->GetColor() != color) {
                    if (pieces[i][k]->LegalMove(pieces, kingX, kingY, i,k)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool AnyValidMoves(char color) {
    //Check every piece
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < height; k++)
        {
            //Is it correct color
            if (pieces[i][k]->GetColor() == color) 
            {
                //Go through every place on the board
                for (int j = 0; j < height; j++)
                {
                    for (int p = 0; p < height; p++)
                    {
                        //If this is a legal move, check for check
                        if (pieces[i][k]->LegalMove(pieces, j, p, i, k)) {
                            //Move for checking if its in check and undo
                            Piece* piece = pieces[j][p];
                            pieces[j][p] = pieces[i][k];
                            pieces[i][k] = 0;
                            if (!IsCheck(color)) {
                                pieces[i][k] = pieces[j][p];
                                pieces[j][p] = piece;
                                return true;
                            }
                            pieces[i][k] = pieces[j][p];
                            pieces[j][p] = piece;
                        }
                    }
                }

            }
        }
    }
    //If none returned a move thats not a check then no moves are avalibale
    return false;
}

void NextPlayer() {
    if (player == WHITE) {
        player = BLACK;
    }
    else {
        player = WHITE;
    }
}
bool GameOver() {
    if (!AnyValidMoves) {
        //If the current player is in check, its a checkmate, else its a draw
        if (IsCheck(player)) {
            //its checkmate
            NextPlayer();
            std::cout << "Game Over! " << player << " wins!";
        }
        else {
            //Draw
            std::cout << "Its a draw!";
        }
        return true;
    }
    else {
        return false;
    }
}
