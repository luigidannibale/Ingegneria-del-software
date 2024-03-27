#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include <wx-3.0/wx/wx.h>
#include "../view/GameplayFrame.h"

class GameplayController {
public:
    GameplayController();
    ~GameplayController();
private:

    GameplayFrame* frame;

    void ClickBoard(wxMouseEvent&);
};


#endif //GAMEPLAYCONTROLLER_H
