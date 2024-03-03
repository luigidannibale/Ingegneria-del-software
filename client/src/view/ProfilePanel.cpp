#include <wx-3.0/wx/wx.h>
#include "ProfilePanel.h"

ProfilePanel::ProfilePanel(wxWindow* parent,User* user): wxPanel(parent)
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    defaultPanel = new wxPanel(this, wxID_ANY);
    updatePanel = new wxPanel(this, wxID_ANY);

    txtUsername = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Username()));
    txtName     = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Name()));
    txtSurname  = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Surname()));
    lblEloPointsU = new wxStaticText(updatePanel, wxID_ANY, wxString("Elo Points : " + std::to_string(user->EloPoints())));

    lblUsername = new wxStaticText(defaultPanel, wxID_ANY, wxString("Username : "+user->Username()));
    lblName     = new wxStaticText(defaultPanel, wxID_ANY, wxString("Name : "+user->Name()));
    lblSurname  = new wxStaticText(defaultPanel, wxID_ANY, wxString("Surname : "+user->Surname()));
    lblEloPointsD = new wxStaticText(defaultPanel, wxID_ANY, wxString("Elo Points : " + std::to_string(user->EloPoints())));

    btnUpdateMode = new wxButton(defaultPanel, wxID_ANY, wxT("Update profile"));
    btnUpdateMode->Bind(wxEVT_BUTTON, &ProfilePanel::ShowUpdate, this);
    btnSave = new wxButton(updatePanel, wxID_ANY, wxT("Save"));
    btnBack = new wxButton(defaultPanel, wxID_ANY, wxT("Back"));
    btnCancel = new wxButton(updatePanel, wxID_ANY, wxT("Cancel"));
    btnCancel->Bind(wxEVT_BUTTON, &ProfilePanel::ShowDefault, this);

    wxBoxSizer* defaultPanelSizer = new wxBoxSizer(wxVERTICAL);
    defaultPanelSizer->Add(lblUsername, 0, wxALL | wxEXPAND, 5);
    defaultPanelSizer->Add(lblName, 0, wxALL | wxEXPAND, 5);
    defaultPanelSizer->Add(lblSurname, 0, wxALL | wxEXPAND, 5);
    defaultPanelSizer->Add(lblEloPointsD, 0, wxALL | wxEXPAND, 5);
    defaultPanelSizer->Add(btnUpdateMode, 0, wxALL | wxALIGN_CENTER, 5);
    defaultPanelSizer->Add(btnBack, 0, wxALL | wxALIGN_CENTER, 5);
    defaultPanel->SetSizerAndFit(defaultPanelSizer);
    defaultPanel->Show();

    wxBoxSizer* updatePanelSizer = new wxBoxSizer(wxVERTICAL);
    updatePanelSizer->Add(txtUsername, 0, wxALL | wxEXPAND, 5);
    updatePanelSizer->Add(txtName, 0, wxALL | wxEXPAND, 5);
    updatePanelSizer->Add(txtSurname, 0, wxALL | wxEXPAND, 5);
    updatePanelSizer->Add(lblEloPointsU, 0, wxALL | wxEXPAND, 5);
    updatePanelSizer->Add(btnSave, 0, wxALL | wxALIGN_CENTER, 5);
    updatePanelSizer->Add(btnCancel, 0, wxALL | wxALIGN_CENTER, 5);
    updatePanel->SetSizerAndFit(updatePanelSizer);
    updatePanel->Hide();

}

void ProfilePanel::ShowUpdate(wxCommandEvent& event) {
    defaultPanel->Hide();
    updatePanel->Show();
}

void ProfilePanel::ShowDefault(wxCommandEvent& event) {
    updatePanel->Hide();
    defaultPanel->Show();
}

wxButton* ProfilePanel::GetBtnBack() { return btnBack; }
wxButton* ProfilePanel::GetBtnCancel() { return btnCancel; }
wxButton* ProfilePanel::GetBtnUpdate() { return btnUpdateMode; }
wxButton* ProfilePanel::GetBtnSave() { return btnSave; }
wxTextCtrl* ProfilePanel::GetTxtUsername() { return txtUsername; }
wxTextCtrl* ProfilePanel::GetTxtName() { return txtName; }
wxTextCtrl* ProfilePanel::GetTxtSurname() { return txtSurname; }

void ProfilePanel::UpdateUserDatas(User *user) {
    lblUsername->SetLabel(wxString("Username : "+user->Username()));
    lblName->SetLabel(wxString("Name : "+user->Name()));
    lblSurname->SetLabel(wxString("Surname : "+user->Surname()));

    txtUsername->SetValue(wxString(user->Username()));
    txtName->SetValue(wxString(user->Name()));
    txtSurname->SetValue(wxString(user->Surname()));
}