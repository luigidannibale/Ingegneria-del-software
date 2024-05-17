#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include "GameManager.h"
#include "../lib.h"
#include "../view/GameplayFrame.h"
#include "../model/GameOptions.h"
#include "../model/chess_pachage/chess.hpp"
#include <map>
#include <cmath>
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
    GameplayController(GameOptions*);
    ~GameplayController();
private:
    //bool isWhite;
    GameManager* gameManager;
    CellCoordinates* clickedCoord = nullptr;
    GameplayFrame* frame;
    //chess::Board board;
    chess::Movelist moves;
    std::map<chess::Square,chess::Move> playableMoves;
    void makeMove(std::string_view);
    void markFeasible(chess::Move);
    void unmarkFeasibles();

    void UpdateChessboard();
    void ClickBoard(wxMouseEvent&);
};


#endif //GAMEPLAYCONTROLLER_H
