#ifndef GAMEMODECONTROLLER_H
#define GAMEMODECONTROLLER_H

#include <stdio.h>
#include "../lib.h"
#include "../view/GameMode.h"
#include "../view/LoginPanel.h"
#include "GameplayController.h"

class GamemodeController
{
    public:
        GamemodeController(wxPanel*);
        ~GamemodeController();


        void ShowPanel();


    private:
        GameMode* panel;
        wxPanel* backPanel;

        bool searching = false;

        GameplayController* gameplayController;

        void addButtonEvents();
        void SearchOpponent(wxCommandEvent&);
        void StartGame(wxCommandEvent&);
        void BackPanel(wxCommandEvent&);
        void ChangePanel(wxPanel*, wxPanel*);
};






#endif // GAMEMODECONTROLLER_H

