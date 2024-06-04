#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "../model/chess_pachage/chess.hpp"
#include "../model/GameOptions.h"
#include "StockfishManager.h"
#include <cmath>
// #include "GameplayController.h"

enum Turn{
    White = true,
    Black = false
};

class GameManager {
public:
    GameManager(GameOptions*);
    ~GameManager();
    void updateBoard(chess::Board);
    chess::Board GetBoard();
    bool isWhite();
    Turn getTurn();
    bool playerCanPlay();
    void makeComputerMove();
    void swapTurn();
    void StartStockfish();
    chess::Move GetBestMove();
private:
    StockfishManager* stockfishManager;
    chess::Board board;
    bool playerIsWhite;
    Turn turn;


};

chess::PieceGenType TypeToGenType(chess::PieceType piece);
#endif //GAMEMANAGER_H
