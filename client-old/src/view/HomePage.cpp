#include "HomePage.h"


HomePage::HomePage(const wxString& title,const wxPoint& pos,const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {
    SetSize(800,600);
    Center();

    homePanel = new wxPanel(this);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    btnPlay = new wxButton(homePanel, wxID_ANY, "Play game");
    btnSettings = new wxButton(homePanel, wxID_ANY, "Settings");
    btnProfile = new wxButton(homePanel, wxID_ANY, "Profile");
    btnViewGame = new wxButton(homePanel, wxID_ANY, "View a game");
    btnExit = new wxButton(homePanel, wxID_ANY, "Exit");

    vbox->Add(btnPlay, wxSizerFlags(1).Expand().Border(wxALL, 10));      // 1 indicates proportion
    vbox->Add(btnSettings, wxSizerFlags(1).Expand().Border(wxALL, 10));
    vbox->Add(btnViewGame, wxSizerFlags(1).Expand().Border(wxALL, 10));
    vbox->Add(btnProfile, wxSizerFlags(1).Expand().Border(wxALL, 10));
    vbox->Add(btnExit, wxSizerFlags(1).Expand().Border(wxALL, 10));

    homePanel->SetSizerAndFit(vbox);

    homePanel->Show();
}

void HomePage::HidePanel() { homePanel->Hide(); }
wxButton* HomePage::GetBtnPlay() { return btnPlay; }
wxButton* HomePage::GetBtnSettings() { return btnSettings; }
wxButton* HomePage::GetBtnProfile() { return btnProfile; }
wxButton* HomePage::GetBtnViewGame() { return btnViewGame; }
wxButton* HomePage::GetBtnExit() { return btnExit; }
wxPanel*  HomePage::GetHomePanel() { return homePanel; }
