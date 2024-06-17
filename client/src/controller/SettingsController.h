#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "../lib.h"
#include "../view/SettingsPanel.h"
#include "../view/GameGraphicOptions.h"

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

