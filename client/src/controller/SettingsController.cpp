#include "SettingsController.h"

SettingsController::SettingsController(wxPanel* parent)
{
    panel = new SettingsPanel(parent->GetParent());
    panel->Show();
    backPanel = parent;
    addButtonEvents();
}

void SettingsController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &SettingsController::BackPanel, this);
}

void SettingsController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

SettingsController::~SettingsController()
{
}