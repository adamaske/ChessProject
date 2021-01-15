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
    std::string input = "";
    Piece* pieceToMove = NULL;
    Piece* temp = NULL;
    int state = 0;
    int x = 0;
    int y = 0;
    int t_x = 0;
    int t_y = 0;
    std::string turn = (player == WHITE) ? "WHITE" : "BLACK";
    bool asking = true;
    while (asking) {
        switch (state)
        {
        case 0:
            system("cls");
            PrintBoard();
            //Ask for what piece to move
            std::cout << "\nIt is " << turn << "'s turn to move!\n\n";
            
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
                system("pause");
                break;
            }
            //Move it and check the move dont set you i check
            temp = pieces[t_x][t_y];
            pieces[x][y] = NULL;
            pieces[t_x][t_y] = pieceToMove;
            if (IsCheck(player)) {
                //Undo move and give error message
                pieces[x][y] = pieceToMove;
                pieces[t_x][t_y] = temp;
                std::cout << "This move would set you in check, illegal!\n";
                system("pause");
                break;
            }
            MakeMove(x, y, t_x, t_y);
            state = 3;
            asking = false;
            //If it gets to this state, return the whole function
            return;
        default:
            return;
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
    int kingX = 0;
    int kingY = 0;

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
                        std::cout << "This piece" << i << k << "has you king in check, with king pos at" << kingX << kingY;
                        system("pause");
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
    Piece* piece = NULL;
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            if (pieces[i][k] != 0) {
                //Is it correct color
                if (pieces[i][k]->GetColor() == color)
                {
                    //Go through every place on the board
                    for (int j = 0; j < height; j++)
                    {
                        for (int p = 0; p < width; p++)
                        {
                            //If this is a legal move, check for check
                            if (pieces[i][k]->LegalMove(pieces, j, p, i, k)) {
                                return true;
                                //Move for checking if its in check and undo
                                piece = pieces[j][p];
                                pieces[j][p] = pieces[i][k];
                                pieces[i][k] = NULL;
                                bool k = IsCheck(color);
                                if (!k) {
                                    //Undo move
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
    if (!AnyValidMoves(player)) {
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

//NEd her er AI
std::string AIMove(char color) {
    int bestScore = -100;
    std::string bestMove = "";
    std::string bestPiece = "";
    Piece* temp = 0;
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            //Check if this piece is my color
            if (pieces[i][k] != NULL) {
                if (pieces[i][k]->GetColor() == color) {
                    for (int j = 0; j < height; j++)
                    {
                        for (int p = 0; p < width; p++) {
                            if (pieces[i][k]->LegalMove(pieces, j, p, i, k)) {
                                temp = pieces[j][p];
                                pieces[j][p] = pieces[i][k];
                                pieces[i][k] = 0;
                                int score = minimax(BLACK);
                                pieces[i][k] = pieces[j][p];
                                pieces[j][p] = temp;
                                if (score > bestScore) {
                                    bestScore = score;
                                    bestPiece[0] = i;
                                    bestPiece[1] = k;
                                    bestMove[0] = j;
                                    bestMove[1] = p;
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
    return bestPiece+bestMove;
}

int minimax(char color) {
    int result = -10;
    if (!AnyValidMoves(color)) {
        //If the current player is in check, its a checkmate, else its a draw
        if (IsCheck(BLACK)) {
            //AI in checkMate
            result = -1;
            return result;
        }
        else if(IsCheck(WHITE)){
            //WHite lost
            result = 1;
            return result;
        }
        else {
            //Draw
            result = 0;
            return result;
        }
    }
    Piece* temp;
    if (color == BLACK) {
        int bestScore = -100;
        for (int i = 0; i < height; i++)
        {
            for (int k = 0; k < width; k++)
            {
                if (pieces[i][k] != NULL) {
                    //Check if this piece is my color
                    if(pieces[i][k]->GetColor() == color) {
                        for (int j = 0; j < height; j++)
                        {
                            for (int p = 0; p < width; p++) {
                                if (pieces[i][k]->LegalMove(pieces, j, p, i, k)) {
                                    temp = pieces[j][p];
                                    pieces[j][p] = pieces[i][k];
                                    pieces[i][k] = 0;
                                   
                                    int score = minimax(BLACK);
                                    
                                    system("pause");
                                    pieces[i][k] = pieces[j][p];
                                    pieces[j][p] = temp;
                                    if (score > bestScore) {
                                        bestScore = score;
                                    }
                                    delete temp;
                                }
                            }
                        }
                    }
                }
                
            }
        }
        
        return bestScore;
    }
    else {
        int bestScore = 100;
        for (int i = 0; i < height; i++)
        {
            for (int k = 0; k < width; k++)
            {
                if (pieces[i][k] != NULL) {
                    //Check if this piece is my color
                    if (pieces[i][k]->GetColor() == WHITE) {
                        for (int j = 0; j < height; j++)
                        {
                            for (int p = 0; p < width; p++) {
                                if (pieces[i][k]->LegalMove(pieces, j, p, i, k)) {
                                    temp = pieces[j][p];
                                    pieces[j][p] = pieces[i][k];
                                    pieces[i][k] = 0;
                                    int score = minimax(WHITE);
                                    pieces[i][k] = pieces[j][p];
                                    pieces[j][p] = temp;
                                    if (score < bestScore) {
                                        bestScore = score;
                                    }
                                    delete temp;
                                }
                            }
                        }
                    }
                }
                
            }
        }
       
        return bestScore;
    }
    return 0;
}
