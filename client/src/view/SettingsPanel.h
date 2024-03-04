#ifndef SETTINGSPANEL_H
#define  SETTINGSPANEL_H

#include <wx-3.0/wx/wx.h>
#include "ImageFuncions.h"

class SettingsPanel : public wxPanel{
public:
    SettingsPanel(wxWindow*);
    wxButton* GetBtnBack();
    wxButton* GetBtnSave();

private:
    wxButton* btnBack;
    wxButton* btnSave;

    void BrownClick(wxMouseEvent&);
    void BlueClick(wxMouseEvent&);
    void BlackClick(wxMouseEvent&);
};

#endif //SETTINGSPANEL_H


