#include "ProfileController.h"


ProfileController::ProfileController(wxPanel* parent)
{
    panel = new ProfilePanel(parent->GetParent());
    panel->Show(true);
    backPanel = parent;

    addButtonEvents();
}

void ProfileController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &ProfileController::BackPanel, this);
}

void ProfileController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

ProfileController::~ProfileController()
{
}