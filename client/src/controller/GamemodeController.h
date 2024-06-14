#ifndef GAMEMODECONTROLLER_H
#define GAMEMODECONTROLLER_H

#include <stdio.h>
#include "../lib.h"
#include "../view/GameMode.h"
#include "../view/LoginPanel.h"
#include "GameplayController.h"
#include "RedisManager.h"

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
        std::string message;
        GameplayController* gameplayController;
        RedisManager* red;

        void addButtonEvents();
        void SearchOpponent(wxCommandEvent&);
        void StopSearchOpponent(wxCommandEvent&);
        void StartGame(wxCommandEvent&);
        void BackPanel(wxCommandEvent&);
        void ChangePanel(wxPanel*, wxPanel*);
};






#endif // GAMEMODECONTROLLER_H

