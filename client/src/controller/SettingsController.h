#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "../lib.h"
#include "../view/SettingsPanel.h"

class SettingsController
{
public:
    SettingsController(wxPanel*);
    ~SettingsController();

    void ShowPanel();

private:
    SettingsPanel* panel;
    wxPanel* backPanel;

    void addButtonEvents();
    void BackPanel(wxCommandEvent&);

};


#endif // SETTINGSCONTROLLER_H

