#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include "../lib.h"
#include "../view/GameplayFrame.h"
#include "../model/GameOptions.h"
#include "../model/chess_pachage/chess.hpp"
#include <map>

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
    bool isWhite;
    CellCoordinates* clickedCoord = nullptr;
    GameplayFrame* frame;
    chess::Board board;
    chess::Movelist moves;
    std::map<chess::Square,chess::Move> playableMoves;
    void ClickBoard(wxMouseEvent&);
};


#endif //GAMEPLAYCONTROLLER_H
