#include <stdio.h>
// #include <wx/wx.h>
#include "../view/HomePage.h"
#include "GamemodeController.h"

class HomeController
{
    public:
        HomeController(/* args */);
        ~HomeController();

    private:
        HomePage* frame;

        void addButtonEvents();
};

// Constructor
HomeController::HomeController(/* args */)
{
    // istanzia frame con l'HomePage
    frame = new HomePage("Gioco degli scacchi",wxPoint(10,20),wxSize(800,600));
    frame->Show(true);

    addButtonEvents();
}

void HomeController::addButtonEvents() {
    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        // wxLogMessage("Play");
        GamemodeController* game = new GamemodeController(frame);
        frame->HidePanel();
    }, ID_btnPlay);

    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        wxLogMessage("Settings");
    }, ID_btnSettings);

    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        wxLogMessage("Profile");
    }, ID_btnProfile);

    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        wxLogMessage("ViewGame");
    }, ID_btnViewGame);

    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        wxLogMessage("Exit");
    }, ID_btnClose);
}

HomeController::~HomeController()
{
}

