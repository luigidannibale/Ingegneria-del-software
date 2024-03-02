#include "HomeController.h"

// Constructor
HomeController::HomeController(/* args */)
{
    // istanzia frame con l'HomePage
    frame = new HomePage("Gioco degli scacchi",wxPoint(10, 20),wxSize(800,600));
    frame->Show(true);

    addButtonEvents();
}

void HomeController::addButtonEvents() {
    frame->GetBtnPlay()->Bind(wxEVT_BUTTON, &HomeController::showGamemode, this);

    frame->GetBtnSettings()->Bind(wxEVT_BUTTON, &HomeController::showSettings, this);

    frame->GetBtnProfile()->Bind(wxEVT_BUTTON, &HomeController::showProfile, this);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("ViewGame");
    // }, ID_btnViewGame);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("Exit");
    // }, ID_btnClose);
}

void HomeController::showGamemode(wxCommandEvent& event){
    GamemodeController* game = new GamemodeController(frame->GetHomePanel());
    frame->HidePanel();
}

void HomeController::showSettings(wxCommandEvent& event){
    // GamemodeController* game = new GamemodeController(frame);
    // frame->HidePanel();
}

void HomeController::showProfile(wxCommandEvent& event){
    // GamemodeController* game = new GamemodeController(frame);
    // frame->HidePanel();
    ProfileController* profile = new ProfileController(frame->GetHomePanel());
    frame->HidePanel();

}

HomeController::~HomeController()
{
}