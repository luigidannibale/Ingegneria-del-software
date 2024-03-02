#include "LoginPanel.h"

LoginPanel::LoginPanel(wxWindow* parent): wxPanel(parent)
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    wxStaticText* usernameLabel = new wxStaticText(this, wxID_ANY, wxT("Username:"));
    txtUsername = new wxTextCtrl(this, wxID_ANY, wxT(""));

    //passwd wxStaticText* passwordLabel = new wxStaticText(this, wxID_ANY, wxT("Password:"));
    //passwd txtPassword = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(-1, -1), wxDefaultSize, wxTE_PASSWORD);

    btnLogin = new wxButton(this, wxID_ANY, wxT("Login"));
    btnBack = new wxButton(this, wxID_ANY, wxT("Back"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(usernameLabel, 0, wxALL | wxEXPAND, 5);
    sizer->Add(txtUsername, 0, wxALL | wxEXPAND, 5);
    //passwd sizer->Add(passwordLabel, 0, wxALL | wxEXPAND, 5);
    //passwd sizer->Add(txtPassword, 0, wxALL | wxEXPAND, 5);
    sizer->Add(btnLogin, 0, wxALL | wxALIGN_CENTER, 5);
    sizer->Add(btnBack, 0, wxALL | wxALIGN_CENTER, 5);
    SetSizerAndFit(sizer);
}

wxString LoginPanel::GetUsername() { return txtUsername->GetValue(); }