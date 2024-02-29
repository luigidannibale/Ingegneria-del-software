#include <stdio.h>
// #include <wx/wx.h>
#include "../view/GameMode.h"
#include "../view/LoginPanel.h"
#include "../view/ProfilePanel.h"

class GamemodeController
{
    public:
        GamemodeController(wxFrame*);
        ~GamemodeController();

    private:
        GameMode* panel;
        LoginPanel* prova;
        ProfilePanel* prova1;
        void addButtonEvents();
};

// Constructor
GamemodeController::GamemodeController(wxFrame* parent)
{
    // istanzia frame con l'HomePage
//    panel = new GameMode(parent);
//    panel->Show(true);
//    prova = new LoginPanel(parent);
//    prova->Show(true);
    prova1 = new ProfilePanel(parent);
    prova1->Show(true);
}

void GamemodeController::addButtonEvents() {
}

GamemodeController::~GamemodeController()
{
}

