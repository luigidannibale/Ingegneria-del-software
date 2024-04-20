#include "GamemodeController.h"


GamemodeController::GamemodeController(wxPanel* parent)
{
    panel = new GameMode(parent->GetParent());
    panel->Hide();
    backPanel = parent;
    addButtonEvents();
}

void GamemodeController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &GamemodeController::BackPanel, this);

    panel->GetBtnComputer()->Bind(wxEVT_BUTTON,  [this](wxCommandEvent& event){
        ChangePanel(panel->GetComputerPanel(),panel->GetMultiplayerPanel());
    });
    panel->GetBtnMultiplayer()->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        ChangePanel(panel->GetMultiplayerPanel(), panel->GetComputerPanel());
    });
    panel->GetBtnStartMultiplayer()->Bind(wxEVT_BUTTON, &GamemodeController::StartGame, this);
    panel->GetBtnStartComputer()->Bind(wxEVT_BUTTON, &GamemodeController::StartGame, this);
}

void SearchOpponent(wxCommandEvent& event) {

}

void GamemodeController::StartGame(wxCommandEvent& event){
    GameOptions* options = panel->GetGameOptions();
//    printf("Against human is %d \n",options->GetAgaintsHuman());
//    printf("Gametime is %d|%d \n",options->GetGameDurationInSeconds(),options->GetGameIncrement());
//    printf("Computer elo is %d \n",options->GetComputerElo());
//    printf("You start with %d \n",options->GetStartSide());
//    printf("---------------------------------------------------\n");
    gameplayController = new GameplayController(options);

//    if (gameplayController == nullptr) {
//        gameplayController = new GameplayController(options);
//    }
//    else {
//        // gameplayController.startaGame ?
//    }
}


void GamemodeController::ChangePanel(wxPanel* show, wxPanel* hide) {
    show->Show();
    hide->Hide();
}

void GamemodeController::ShowPanel() {
    panel->Show();
}

void GamemodeController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->GetComputerPanel()->Hide();
    panel->GetMultiplayerPanel()->Hide();
    panel->Hide();
}

GamemodeController::~GamemodeController()
{

}