#include <stdio.h>
// #include <wx/wx.h>
#include "../view/GameMode.h"
#include "../view/LoginPanel.h"

class GamemodeController
{
    public:
        GamemodeController(wxPanel*);
        ~GamemodeController();

    private:
        GameMode* panel;
        LoginPanel* prova;
        // ProfilePanel* prova1;
        wxPanel* backPanel;

        void addButtonEvents();

        void BackPanel(wxCommandEvent&);
};

// Constructor
GamemodeController::GamemodeController(wxPanel* parent)
{
    // istanzia frame con l'HomePage
   panel = new GameMode(parent->GetParent());
   panel->Show(true);
   backPanel = parent;
//    prova = new LoginPanel(parent);
//    prova->Show(true);
    // prova1 = new ProfilePanel(parent);
    // prova1->Show(true);

    addButtonEvents();
}

void GamemodeController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &GamemodeController::BackPanel, this);
}

void GamemodeController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

GamemodeController::~GamemodeController()
{
}

