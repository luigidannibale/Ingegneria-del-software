#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include "GameManager.h"
#include "RedisManager.h"
#include "../lib.h"
#include "../view/GameplayFrame.h"
#include "../model/GameOptions.h"
#include "../model/chess_pachage/chess.hpp"

#include <map>
#include <wx/utils.h>
#include <thread>
#include <random>
#include <string_view>

struct CellCoordinates{
public:
    CellCoordinates(int row, int col, std::string coordinates) {
        this->row = row;
        this->col = col;
        this->coordinates = coordinates;
    }
    int row;
    int col;
    std::string coordinates;
};

class GameplayController {
public:
    GameplayController(GameOptions*, RedisManager* = nullptr, std::string channel = "");
    ~GameplayController();

private:
    //bool isWhite;
    bool gameClosed = false;
    bool gameEnded = false;
    GameManager* gameManager;
    RedisManager* redisManager;
    std::string channel;
    CellCoordinates* clickedCoord = nullptr;
    GameplayFrame* frame;

    const int MS_STOCKFISH_DELAY = 2000;

    //chess::Board board;
    chess::Movelist moves;
    std::map<chess::Square,chess::Move> playableMoves;
    void makeMove(std::string_view);
    void markFeasible(chess::Move);
    void unmarkFeasibles();
    bool CheckCheckmate();
    void AsyncComputerMove();
    void PublishHumanMove(std::string);
    void AsyncHumanMove();

    void UpdateChessboard();
    void OnClose(wxCloseEvent&);
    void ClickBoard(wxMouseEvent&);
    void UpdateTime(wxTimerEvent&);
};



#endif //GAMEPLAYCONTROLLER_H
