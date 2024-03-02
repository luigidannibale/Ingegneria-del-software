#ifndef HOMEPAGE_H
#define  HOMEPAGE_H

#include <wx-3.0/wx/wx.h>
#include <stdio.h>
#include <stdlib.h>

class HomePage : public wxFrame {
public:
    HomePage(const wxString&,const wxPoint&,const wxSize&);

    wxButton* GetBtnPlay();
    wxButton* GetBtnSettings();
    wxButton* GetBtnProfile();
    wxButton* GetBtnViewGame();
    wxButton* GetBtnExit();
    wxPanel* GetHomePanel();
    void HidePanel();

private:
    wxPanel* homePanel;
    wxButton* btnPlay;
    wxButton* btnSettings;
    wxButton* btnProfile;
    wxButton* btnViewGame;
    wxButton* btnExit;

};
#endif // HOMEPAGE_H


