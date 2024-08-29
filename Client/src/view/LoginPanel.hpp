#ifndef LOGINPANEL_H
#define  LOGINPANEL_H

#include "../lib.hpp"


class LoginPanel : public wxPanel{
public:
    LoginPanel(wxWindow* parent);
    // wxString GetUsername();
    
    wxTextCtrl* getUsername();
    wxButton* getLoginButton();

    void ShowError(const wxString &message);

private:
    wxTextCtrl* txtUsername;
    wxStaticText* errorLabel;
    wxButton* btnLogin;
    
};

#endif // LOGINPANEL_H