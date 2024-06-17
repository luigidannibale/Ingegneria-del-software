#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "../lib.h"
#include "../model/GameOptions.h"
#include "GameGraphicOptions.h"

class GameMode : public wxPanel {
public:
    GameMode(wxWindow* parent);
    
    wxButton* GetBtnMultiplayer();
    wxButton* GetBtnComputer();
    wxButton* GetBtnBack();
    wxButton* GetBtnStartMultiplayer();
    wxButton* GetBtnStartComputer();
    wxButton* GetBtnQuitMultiplayer();
    wxPanel* GetMultiplayerPanel();
    wxPanel* GetComputerPanel();
    GameOptions* GetGameOptions();
    GameGraphicOptions* GetGameGraphicOptions();

private:
    wxButton* btnMultiplayer;
    wxButton* btnComputer;
    wxButton* btnBack;

    wxButton* btnStartComputer;
    wxButton* btnStartMultiplayer;
    wxButton* btnQuitMultiplayer;

    wxPanel* multiplayerPanel;
    wxPanel* computerPanel;

    int selectedDuration;
    int selectedDifficulty;
    int selectedSide;
    wxRadioBox* multiplayerTimeBox;
    wxRadioBox* computerTimeBox;
    wxRadioBox* difficultyBox;
    wxRadioBox* startSideBox;

    GameTime* GetGameTime();

    void initPanel(wxPanel*);
};



#endif //GAMEMODE_H


