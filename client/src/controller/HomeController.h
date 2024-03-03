#ifndef  HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <stdio.h>
#include <wx-3.0/wx/wx.h>
#include "../view/HomePage.h"
#include "GamemodeController.h"
#include "ProfileController.h"
#include "SettingsController.h"

class HomeController
{
    public:
        HomeController(/* args */);
        ~HomeController();

    private:
        HomePage* frame;

        void addButtonEvents();

        void showGamemode(wxCommandEvent&);
        void showSettings(wxCommandEvent&);
        void showProfile(wxCommandEvent&);
        void Exit(wxCommandEvent&);
};

#endif // HOMECONTROLLER_H

