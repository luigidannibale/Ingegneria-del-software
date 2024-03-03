#include "GamemodeController.h"
GamemodeController::GamemodeController(wxPanel* parent)
{
    printf("GAMEMODE INSTANZIATO\n");
    panel = new GameMode(parent->GetParent());
    panel->Show(true);
    backPanel = parent;
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