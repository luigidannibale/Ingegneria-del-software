#include "ProfilePanel.h"

ProfilePanel::ProfilePanel(wxWindow* parent): wxPanel(parent)
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));


    txtUsername = new wxTextCtrl(this, wxID_ANY, wxT("Some value"));
    txtName = new wxTextCtrl(this, wxID_ANY, wxT("Some value"));
    txtSurname = new wxTextCtrl(this, wxID_ANY, wxT("Some value"));

    //passwd wxStaticText* passwordLabel = new wxStaticText(this, wxID_ANY, wxT("Password:"));
    //passwd txtPassword = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(-1, -1), wxDefaultSize, wxTE_PASSWORD);

    btnUpdateMode = new wxButton(this, wxID_ANY, wxT("Update profile"));
    btnSave = new wxButton(this, wxID_ANY, wxT("Save"));
    btnBack = new wxButton(this, wxID_ANY, wxT("Back"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(txtUsername, 0, wxALL | wxEXPAND, 5);
    sizer->Add(txtName, 0, wxALL | wxEXPAND, 5);
    sizer->Add(txtSurname, 0, wxALL | wxEXPAND, 5);
    sizer->Add(lblEloPoints, 0, wxALL | wxEXPAND, 5);
    //passwd sizer->Add(passwordLabel, 0, wxALL | wxEXPAND, 5);
    //passwd sizer->Add(txtPassword, 0, wxALL | wxEXPAND, 5);
    sizer->Add(btnSave, 0, wxALL | wxALIGN_CENTER, 5);
    sizer->Add(btnUpdateMode, 0, wxALL | wxALIGN_CENTER, 5);
    sizer->Add(btnBack, 0, wxALL | wxALIGN_CENTER, 5);
    SetSizerAndFit(sizer);
}

wxButton* ProfilePanel::GetBtnBack() { return btnBack; }