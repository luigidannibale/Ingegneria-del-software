#include "SettingsController.hpp"


SettingsController::SettingsController(wxPanel* parent)
{
    configuration = new GameGraphicOptions(BoardStyle::brown,PiecesStyle::neo);
    GameGraphicOptions* viewConfiguration = new GameGraphicOptions(configuration->GetBoardStyle(),configuration->GetPiecesStyle());
    panel = new SettingsPanel(parent->GetParent(), viewConfiguration);
    panel->Hide();
    backPanel = parent;
    addButtonEvents();
}

void SettingsController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &SettingsController::BackPanel, this);
    panel->GetBtnSave()->Bind(wxEVT_BUTTON, &SettingsController::SaveSettings, this);
}

void SettingsController::ShowPanel() {
    panel->Show();
}

void SettingsController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
    panel->SetGameGraphicOptions(configuration);
}

void SettingsController::SaveSettings(wxCommandEvent& event) {
    configuration = panel->GetGameGraphicOptions();
    backPanel->Show();
    panel->Hide();
}

GameGraphicOptions* SettingsController::GetGameGraphicOptions() {
    return configuration;
}

SettingsController::~SettingsController()
{
}