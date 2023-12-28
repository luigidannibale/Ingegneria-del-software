#include <wx/wx.h>
#include "MatchmakingPanel.h"

enum {
    ID_btnMultiplayer = 1,
    ID_btnComputer,
    ID_btnFriend,
    ID_btnBack
};


class GameMode : public wxPanel {
public:
    GameMode(wxWindow* parent, wxPanel* backPanel); 
    
    wxButton* GetBtnMultiplayer()    { return btnMultiplayer; }
    wxButton* GetBtnComputer()       { return btnComputer; }
    wxButton* GetBtnFriend()         { return btnFriend; }
    wxButton* GetBtnBack()           { return btnBack; }

private:
    // void btnMultiplayer(wxCommandEvent& event);
    // void btnComputer(wxCommandEvent& event);
    // void btnFriend(wxCommandEvent& event);
    // void btnBack(wxCommandEvent& event);

    wxButton* btnMultiplayer;
    wxButton* btnComputer;
    wxButton* btnFriend;
    wxButton* btnBack;

    // wxPanel* backPanel;
    
    // wxDECLARE_EVENT_TABLE();
};

// wxBEGIN_EVENT_TABLE(GameMode, wxPanel)
    // EVT_BUTTON(ID_btnMultiplayer, GameMode::btnMultiplayer)
    // EVT_BUTTON(ID_btnComputer, GameMode::btnComputer)
    // EVT_BUTTON(ID_btnFriend, GameMode::btnFriend)  
    // EVT_BUTTON(ID_btnBack, GameMode::btnBack)
// wxEND_EVENT_TABLE()


GameMode::GameMode(wxWindow* parent, wxPanel* panel): wxPanel(parent) { 
    backPanel = panel;
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));
    
    btnMultiplayer = new wxButton(this, ID_btnMultiplayer, "Gioca una partita multigiocatore",wxPoint(10,50));
    btnComputer = new wxButton(this, ID_btnComputer, "Gioca una partita contro il computer",wxPoint(10,100));
    btnFriend = new wxButton(this, ID_btnFriend, "Sfida un amico",wxPoint(10,150));
    btnBack = new wxButton(this, ID_btnBack, "<-",wxPoint(10,200));
    
    // Bind(wxEVT_BUTTON, &GameMode::btnMultiplayer, this, ID_btnMultiplayer);
    // Bind(wxEVT_BUTTON, &GameMode::btnComputer, this, ID_btnComputer);
    // Bind(wxEVT_BUTTON, &GameMode::btnFriend, this, ID_btnFriend);
    // Bind(wxEVT_BUTTON, &GameMode::btnBack, this, ID_btnBack);
}

// void GameMode::btnMultiplayer(wxCommandEvent& event) {    
//     MatchmakingPanel * matchmakingPanel = new MatchmakingPanel(parent,this);
//     matchmakingPanel->Show();
//     Hide()
// }
// void GameMode::btnComputer(wxCommandEvent& event) {    
//     wxLogMessage("Computer");
// }
// void GameMode::btnFriend(wxCommandEvent& event) {    
//     wxLogMessage("Friend");
// }

// void GameMode::btnBack(wxCommandEvent& event) {    
//     backPanel->Show();
//     Hide();
//     Destroy();
// }