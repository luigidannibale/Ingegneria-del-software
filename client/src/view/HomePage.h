#ifndef HOMEPAGE_H
#define  HOMEPAGE_H

#include <stdio.h>
#include <stdlib.h>
#include "../lib.h"

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


