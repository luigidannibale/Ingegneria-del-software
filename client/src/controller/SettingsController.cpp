#include "SettingsController.h"


SettingsController::SettingsController(wxPanel* parent)
{
    GameGraphicOptions* configuration = new GameGraphicOptions(BoardStyle::brown,PiecesStyle::neo);
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