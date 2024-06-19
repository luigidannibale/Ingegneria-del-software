#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "../lib.hpp"
#include "../view/SettingsPanel.hpp"
#include "../view/GameGraphicOptions.hpp"

class SettingsController
{
public:
    SettingsController(wxPanel*);
    ~SettingsController();

    void ShowPanel();
    GameGraphicOptions* GetGameGraphicOptions();

private:
    SettingsPanel* panel;
    wxPanel* backPanel;

    GameGraphicOptions* configuration;

    void addButtonEvents();
    void BackPanel(wxCommandEvent&);
    void SaveSettings(wxCommandEvent&);

};


#endif // SETTINGSCONTROLLER_H

