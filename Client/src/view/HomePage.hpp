#ifndef HOMEPAGE_H
#define  HOMEPAGE_H

#include <cstdio>
#include <cstdlib>
#include "../lib.hpp"

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


