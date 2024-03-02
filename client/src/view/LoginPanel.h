#ifndef LOGINPANEL_H
#define  LOGINPANEL_H

#include <wx-3.0/wx/wx.h>

class LoginPanel : public wxPanel{
public:
    LoginPanel(wxWindow* parent);
    wxString GetUsername();
    //passwd wxString GetPassword() { return txtPassword->GetValue(); }

private:
    wxTextCtrl* txtUsername;
    //passwd wxTextCtrl* txtPassword;
    wxButton* btnLogin;
    wxButton* btnBack;
};

#endif // LOGINPANEL_H