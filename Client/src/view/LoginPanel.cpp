#include "LoginPanel.hpp"
#include <iostream>

LoginPanel::LoginPanel(wxWindow *parent) : wxPanel(parent)
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118, 150, 86));

    wxStaticText *usernameLabel = new wxStaticText(this, wxID_ANY, wxT("Enter username:"));
    usernameLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    usernameLabel->SetMinSize(usernameLabel->GetTextExtent(usernameLabel->GetLabel()));
    txtUsername = new wxTextCtrl(this, wxID_ANY, wxT(""));

    btnLogin = new wxButton(this, wxID_ANY, wxT("Login"));

    errorLabel = new wxStaticText(this, wxID_ANY, wxT("Server error"));
    errorLabel->SetForegroundColour(wxColour(255, 0, 0));
    errorLabel->Hide();

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer();
    sizer->Add(usernameLabel, wxSizerFlags(0).Center().Border(wxALL, 5));
    sizer->Add(txtUsername, wxSizerFlags(0).Center().Expand().Border(wxALL, 5));
    sizer->Add(btnLogin, wxSizerFlags(0).Center().Expand().Border(wxALL, 5));
    sizer->Add(errorLabel, wxSizerFlags(0).Center().Border(wxALL, 5));
    sizer->AddStretchSpacer();
    SetSizer(sizer);
    Layout();
}

wxButton *LoginPanel::getLoginButton() { return btnLogin; }
wxTextCtrl *LoginPanel::getUsername() { return txtUsername; }

void LoginPanel::ShowError(const wxString &message)
{
    errorLabel->SetLabel(message);
    errorLabel->Show();
    Layout();
}

// wxString LoginPanel::GetUsername() { return txtUsername->GetValue(); }