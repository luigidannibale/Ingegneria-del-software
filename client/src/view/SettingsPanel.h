#ifndef SETTINGSPANEL_H
#define  SETTINGSPANEL_H

#include <wx-3.0/wx/wx.h>

class SettingsPanel : public wxPanel{
public:
    SettingsPanel(wxWindow*);
    wxButton* GetBtnBack();
    wxButton* GetBtnSave();

private:
    wxButton* btnBack;
    wxButton* btnSave;
};


#endif //SETTINGSPANEL_H


