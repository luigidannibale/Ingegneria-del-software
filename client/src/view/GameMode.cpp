#include "GameMode.h"

GameMode::GameMode(wxWindow* parent): wxPanel(parent) {

    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    btnMultiplayer = new wxButton(this, wxID_ANY, "Gioca una partita multigiocatore",wxPoint(10,50));
    btnComputer = new wxButton(this, wxID_ANY, "Gioca una partita contro il computer",wxPoint(10,100));
    btnFriend = new wxButton(this, wxID_ANY, "Sfida un amico",wxPoint(10,150));
    btnBack = new wxButton(this, wxID_ANY, "<-",wxPoint(10,200));
}


wxButton* GameMode::GetBtnMultiplayer()    { return btnMultiplayer; }
wxButton* GameMode::GetBtnComputer()       { return btnComputer; }
wxButton* GameMode::GetBtnFriend()         { return btnFriend; }
wxButton* GameMode::GetBtnBack()           { return btnBack; }