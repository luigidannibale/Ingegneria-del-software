#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include <wx-3.0/wx/wx.h>
#include "../view/GameplayFrame.h"
#include "../model/GameOptions.h"

struct CellCoordinates{
public:
    CellCoordinates(int row, int col, char coordinates[3]) {
        this->row = row;
        this->col = col;
        this->coordinates = coordinates;
    }

    int row;
    int col;
    char* coordinates;
};

class GameplayController {
public:
    GameplayController(GameOptions*);
    ~GameplayController();
private:
    bool isWhite;

    CellCoordinates* clickedCoord = nullptr;
    GameplayFrame* frame;

    void ClickBoard(wxMouseEvent&);
};


#endif //GAMEPLAYCONTROLLER_H
