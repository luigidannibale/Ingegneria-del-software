#include <stdio.h>
#include <wx/wx.h>
#include "../view/HomePage.h"

class Controller
{
    public:
        Controller(/* args */);
        ~Controller();

    private:
        wxFrame* frame;

        void addButtonEvents();
};

// Constructor
Controller::Controller(/* args */)
{
    // istanzia frame con l'HomePage
    frame = new HomePage("Gioco degli scacchi",wxPoint(10,20),wxSize(800,600));
    frame->Show(true);

    addButtonEvents();
}

void Controller::addButtonEvents() {
    frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
        wxLogMessage("Play");
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

// Destructor (Called when the object goes out of scope)
Controller::~Controller()
{
}

