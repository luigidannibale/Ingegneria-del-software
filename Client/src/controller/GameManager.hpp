#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "../model/chess_pachage/chess.hpp"
#include "../model/GameOptions.hpp"
#include "StockfishManager.hpp"
#include <cmath>

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
    bool isAgainstHuman();
    void makeComputerMove();
    void swapTurn();
    void StartStockfish();
    chess::Move GetBestMove();
private:
    bool againstHuman;
    StockfishManager* stockfishManager;
    chess::Board board;
    bool playerIsWhite;
    Turn turn;
    int depthLevel;

};

chess::PieceGenType TypeToGenType(chess::PieceType piece);
#endif //GAMEMANAGER_H
