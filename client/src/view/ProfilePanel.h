#ifndef PROFILEPANEL_H
#define  PROFILEPANEL_H

#include <wx-3.0/wx/wx.h>

class ProfilePanel : public wxPanel{
public:
    ProfilePanel(wxWindow* parent);

    wxButton* GetBtnBack();

private:
    wxTextCtrl* txtUsername;
    wxTextCtrl* txtName;
    wxTextCtrl* txtSurname;
    wxStaticText* lblEloPoints = new wxStaticText(this, wxID_ANY, wxT("Elo points : "));;
    //passwd wxTextCtrl* txtPassword;

    wxButton* btnUpdateMode;
    wxButton* btnSave;
    wxButton* btnBack;

};

#endif //PROFILEPANEL_H
