#include "ProfilePanel.hpp"

ProfilePanel::ProfilePanel(wxWindow *parent, User *user) : wxPanel(parent->GetParent())
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118, 150, 86));

    defaultPanel = new wxPanel(this, wxID_ANY);
    defaultPanel->SetSize(GetSize());
    updatePanel = new wxPanel(this, wxID_ANY);
    updatePanel->SetSize(GetSize());

    wxStaticText *text = new wxStaticText(defaultPanel, wxID_ANY, "Profile", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    text->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    text->SetMinSize(text->GetTextExtent(text->GetLabel()));

    wxStaticText *updateText = new wxStaticText(updatePanel, wxID_ANY, "Update Profile", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    updateText->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    updateText->SetMinSize(updateText->GetTextExtent(updateText->GetLabel()));

    txtUsername = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Username()));
    txtName = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Name()));
    txtSurname = new wxTextCtrl(updatePanel, wxID_ANY, wxString(user->Surname()));
    lblEloPointsU = new wxStaticText(updatePanel, wxID_ANY, wxString("Elo Points : " + std::to_string(user->EloPoints())));

    lblUsername = new wxStaticText(defaultPanel, wxID_ANY, wxString("Username : " + user->Username()));
    lblName = new wxStaticText(defaultPanel, wxID_ANY, wxString("Name : " + user->Name()));
    lblSurname = new wxStaticText(defaultPanel, wxID_ANY, wxString("Surname : " + user->Surname()));
    lblEloPointsD = new wxStaticText(defaultPanel, wxID_ANY, wxString("Elo Points : " + std::to_string(user->EloPoints())));

    btnUpdateMode = new wxButton(defaultPanel, wxID_ANY, wxT("Update profile"));
    btnSave = new wxButton(updatePanel, wxID_ANY, wxT("Save"));
    btnBack = new wxButton(defaultPanel, wxID_ANY, wxT("Back"));
    btnCancel = new wxButton(updatePanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer *defaultPanelSizer = new wxBoxSizer(wxVERTICAL);
    defaultPanelSizer->Add(text, wxSizerFlags(0).Expand().Border(wxALL, 5));
    defaultPanelSizer->Add(lblUsername, wxSizerFlags(0).Expand().Border(wxALL, 5));
    defaultPanelSizer->Add(lblName, wxSizerFlags(0).Expand().Border(wxALL, 5));
    defaultPanelSizer->Add(lblSurname, wxSizerFlags(0).Expand().Border(wxALL, 5));
    defaultPanelSizer->Add(lblEloPointsD, wxSizerFlags(0).Expand().Border(wxALL, 5));
    defaultPanelSizer->Add(btnUpdateMode, wxSizerFlags(0).Border(wxALL, 5));
    defaultPanelSizer->Add(btnBack, wxSizerFlags(0).Border(wxALL, 5));
    defaultPanel->SetSizer(defaultPanelSizer);
    defaultPanel->Show();
    defaultPanel->Layout();

    wxBoxSizer *updatePanelSizer = new wxBoxSizer(wxVERTICAL);
    updatePanelSizer->Add(updateText, wxSizerFlags(0).Expand().Border(wxALL, 5));
    updatePanelSizer->Add(txtUsername, wxSizerFlags(0).Border(wxALL, 5));
    updatePanelSizer->Add(txtName, wxSizerFlags(0).Border(wxALL, 5));
    updatePanelSizer->Add(txtSurname, wxSizerFlags(0).Border(wxALL, 5));
    updatePanelSizer->Add(lblEloPointsU, wxSizerFlags(0).Expand().Border(wxALL, 5));
    updatePanelSizer->Add(btnSave, wxSizerFlags(0).Border(wxALL, 5));
    updatePanelSizer->Add(btnCancel, wxSizerFlags(0).Border(wxALL, 5));
    updatePanel->SetSizer(updatePanelSizer);
    updatePanel->Hide();
    updatePanel->Layout();
}

void ProfilePanel::ShowUpdate()
{
    defaultPanel->Hide();
    updatePanel->Show();
}

void ProfilePanel::ShowDefault()
{
    updatePanel->Hide();
    defaultPanel->Show();
}

wxButton *ProfilePanel::GetBtnBack() { return btnBack; }
wxButton *ProfilePanel::GetBtnCancel() { return btnCancel; }
wxButton *ProfilePanel::GetBtnUpdate() { return btnUpdateMode; }
wxButton *ProfilePanel::GetBtnSave() { return btnSave; }
wxTextCtrl *ProfilePanel::GetTxtUsername() { return txtUsername; }
wxTextCtrl *ProfilePanel::GetTxtName() { return txtName; }
wxTextCtrl *ProfilePanel::GetTxtSurname() { return txtSurname; }

void ProfilePanel::UpdateUserDatas(User *user)
{
    lblUsername->SetLabel(wxString("Username : " + user->Username()));
    lblName->SetLabel(wxString("Name : " + user->Name()));
    lblSurname->SetLabel(wxString("Surname : " + user->Surname()));

    txtUsername->SetValue(wxString(user->Username()));
    txtName->SetValue(wxString(user->Name()));
    txtSurname->SetValue(wxString(user->Surname()));
}