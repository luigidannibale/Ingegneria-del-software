#ifndef  HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <cstdio>
#include "../lib.hpp"
#include "../view/HomePage.hpp"
#include "GamemodeController.hpp"
#include "ProfileController.hpp"
#include "SettingsController.hpp"

class HomeController
{
    public:
        HomeController(/* args */);
        ~HomeController();

    private:
        HomePage* frame;

        GamemodeController* gamemodeController;
        SettingsController* settingsController;
        ProfileController* profileController;

        void addButtonEvents();

        void showGamemode(wxCommandEvent&);
        void showSettings(wxCommandEvent&);
        void showProfile(wxCommandEvent&);
        void Exit(wxCommandEvent&);
};

#endif // HOMECONTROLLER_H

