#ifndef GAMEMODECONTROLLER_H
#define GAMEMODECONTROLLER_H

#include <stdio.h>
#include <wx-3.0/wx/wx.h>
#include "../view/GameMode.h"
#include "../view/LoginPanel.h"

class GamemodeController
{
    public:
        GamemodeController(wxPanel*);
        ~GamemodeController();


        void ShowPanel();


    private:
        GameMode* panel;
        wxPanel* backPanel;


        void addButtonEvents();
        void StartGame(wxCommandEvent& event);
        void BackPanel(wxCommandEvent&);
        void ChangePanel(wxPanel* show, wxPanel* hide);
};






#endif // GAMEMODECONTROLLER_H

