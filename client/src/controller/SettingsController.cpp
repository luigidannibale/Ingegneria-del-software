#include "SettingsController.h"
#include "../model/SettingsConfiguration.h"

SettingsController::SettingsController(wxPanel* parent)
{
    SettingsConfiguration* configuration = new SettingsConfiguration(Chessboard::Brown,Pieces::p1);
    panel = new SettingsPanel(parent->GetParent(), configuration);
    panel->Hide();
    backPanel = parent;
    addButtonEvents();
}

void SettingsController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &SettingsController::BackPanel, this);
}

void SettingsController::ShowPanel() {
    panel->Show();
}

void SettingsController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

SettingsController::~SettingsController()
{
}