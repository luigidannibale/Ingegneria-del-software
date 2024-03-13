#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <wx-3.0/wx/wx.h>
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

