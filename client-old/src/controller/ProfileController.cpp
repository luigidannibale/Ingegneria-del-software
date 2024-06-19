#include "ProfileController.h"
#include "../model/User.h"

ProfileController::ProfileController(wxPanel* parent)
{
    User* user = new User("gigi","Luigi","D'annibale",1300);
    panel = new ProfilePanel(parent->GetParent(),user);
    panel->Hide();
    backPanel = parent;

    addButtonEvents();
}

void ProfileController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &ProfileController::BackPanel, this);
    panel->GetBtnSave()->Bind(wxEVT_BUTTON, &ProfileController::SaveUser, this);
}

void ProfileController::ShowPanel(){
    panel->Show();
}

void ProfileController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

void ProfileController::SaveUser(wxCommandEvent& event) {
    wxLogMessage("SAVE THE USER");
}

ProfileController::~ProfileController()
{
}