#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <wx-3.0/wx/wx.h>
#include "../model/GameOptions.h"

class GameMode : public wxPanel {
public:
    GameMode(wxWindow* parent);
    
    wxButton* GetBtnMultiplayer();
    wxButton* GetBtnComputer();
    wxButton* GetBtnBack();
    wxButton* GetBtnStartMultiplayer();
    wxButton* GetBtnStartComputer();
    wxPanel* GetMultiplayerPanel();
    wxPanel* GetComputerPanel();
    GameOptions* GetGameOptions();

private:
    wxButton* btnMultiplayer;
    wxButton* btnComputer;
    wxButton* btnBack;

    wxButton* btnStartComputer;
    wxButton* btnStartMultiplayer;

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


