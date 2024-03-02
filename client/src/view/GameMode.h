#ifndef GAMEMODE_H
#define  GAMEMODE_H

#include <wx-3.0/wx/wx.h>

class GameMode : public wxPanel {
public:
    GameMode(wxWindow* parent);
    
    wxButton* GetBtnMultiplayer();
    wxButton* GetBtnComputer();
    wxButton* GetBtnFriend();
    wxButton* GetBtnBack();

private:
    wxButton* btnMultiplayer;
    wxButton* btnComputer;
    wxButton* btnFriend;
    wxButton* btnBack;

};

#endif //GAMEMODE_H

