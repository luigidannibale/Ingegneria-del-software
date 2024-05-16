#ifndef INGEGERIA_DEGLI_SCACCHI_GAMEMANAGER_H
#define INGEGERIA_DEGLI_SCACCHI_GAMEMANAGER_H
#include "../model/chess_pachage/chess.hpp"
#include "../model/GameOptions.h"
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
    void makeMove();
    void swapTurn();
private:
    chess::Board board;
    bool playerIsWhite;
    Turn turn;

};


#endif //INGEGERIA_DEGLI_SCACCHI_GAMEMANAGER_H
