#include <wx/wx.h>
#include <stdio.h>
#include <stdlib.h>
// #include "GameMode.h"

enum {
    ID_Hello = 1,
    ID_btnPlay,
    ID_btnSettings,
    ID_btnProfile,
    ID_btnClose,
    ID_btnViewGame
};

class HomePage : public wxFrame {
public:
    HomePage(const wxString& title, const wxPoint& pos, const wxSize& size);

    wxButton* GetBtnPlay() const { return btnPlay; }
    wxButton* GetBtnSettings() const { return btnSettings; }
    wxButton* GetBtnProfile() const { return btnProfile; }
    wxButton* GetBtnViewGame() const { return btnViewGame; }
    wxButton* GetBtnExit() const { return btnExit; }
    void HidePanel() const { homePanel->Hide(); }

private:
    // void btnPlay(wxCommandEvent& event);
    // void btnSettings(wxCommandEvent& event);
    // void btnProfile(wxCommandEvent& event);
    // void btnClose(wxCommandEvent& event);
    // void btnViewGame(wxCommandEvent& event);

    wxPanel* homePanel;

    wxButton* btnPlay;
    wxButton* btnSettings;
    wxButton* btnProfile;
    wxButton* btnViewGame;
    wxButton* btnExit;
    // wxDECLARE_EVENT_TABLE();
};

// wxBEGIN_EVENT_TABLE(HomePage, wxFrame)
//     EVT_BUTTON(ID_btnPlay, HomePage::btnPlay)
//     EVT_BUTTON(ID_btnSettings, HomePage::btnSettings)
//     EVT_BUTTON(ID_btnProfile, HomePage::btnProfile)
//     EVT_BUTTON(ID_btnClose, HomePage::btnClose)
//     EVT_BUTTON(ID_btnViewGame, HomePage::btnViewGame)
// wxEND_EVENT_TABLE()

HomePage::HomePage(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame(NULL, wxID_ANY, title, pos, size) {
    SetSize(800,600);
    Center();

    homePanel = new wxPanel(this);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxButton* btnPlay = new wxButton(homePanel, ID_btnPlay, "Play game");
    wxButton* btnSettings = new wxButton(homePanel, ID_btnSettings, "Settings");
    wxButton* btnProfile = new wxButton(homePanel, ID_btnProfile, "Profile");
    wxButton* btnViewGame = new wxButton(homePanel, ID_btnViewGame, "View a game");
    wxButton* btnExit = new wxButton(homePanel, ID_btnClose, "Exit");

    // Add buttons to the vertical sizer
    vbox->Add(btnPlay, 1, wxEXPAND | wxALL, 10);      // 1 indicates proportion
    vbox->Add(btnSettings, 1, wxEXPAND | wxALL, 10);
    vbox->Add(btnViewGame, 1, wxEXPAND | wxALL, 10);
    vbox->Add(btnProfile, 1, wxEXPAND | wxALL, 10);
    vbox->Add(btnExit, 1, wxEXPAND | wxALL, 10);

    // Center the sizer horizontally and vertically in the frame
    homePanel->SetSizerAndFit(vbox);

    // Bind(wxEVT_BUTTON, &HomePage::btnPlay, this, ID_btnPlay);
    // Bind(wxEVT_BUTTON, &HomePage::btnSettings, this, ID_btnSettings);
    // Bind(wxEVT_BUTTON, &HomePage::btnProfile, this, ID_btnProfile);
    // Bind(wxEVT_BUTTON, &HomePage::btnViewGame, this, ID_btnViewGame);
    // Bind(wxEVT_BUTTON, &HomePage::btnClose, this, ID_btnClose);

    homePanel->Show();
}





// void HomePage::btnPlay(wxCommandEvent& event){
//     //wxLogMessage("Gioca");    

//     // GameMode *frame = new GameMode("Choose the gamemode!", wxPoint(50, 50), wxSize(450, 340));
//     GameMode *gameModePanel = new GameMode(this, homePanel);
//     homePanel->Hide();
//     gameModePanel->Show();
//     std::cout << "PROVA?" << std::endl;
//     // printf(gameModePanel->GetSize());
//     gameModePanel->Layout();
 
//     //Show(false);
// }
// void HomePage::btnSettings(wxCommandEvent& event) {    
//     wxLogMessage("Impostazioni");
// }
// void HomePage::btnProfile(wxCommandEvent& event) {
//     wxLogMessage("Profilo");
// }
// void HomePage::btnViewGame(wxCommandEvent& event) {
//     wxLogMessage("Vedi un game concluso");
// }
// void HomePage::btnClose(wxCommandEvent& event) {
//     Close(true);
// }