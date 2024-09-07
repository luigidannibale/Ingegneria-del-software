#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <cstdio>
#include "../lib.hpp"
#include "../view/HomePage.hpp"
#include "../model/User.hpp"
#include "GamemodeController.hpp"
#include "ProfileController.hpp"
#include "SettingsController.hpp"
#include "LoginController.hpp"
#include "ViewGameController.hpp"

class HomeController
{
public:
    HomeController(/* args */);
    ~HomeController();

private:
    HomePage *frame;

    LoginController *loginController;
    GamemodeController *gamemodeController;
    SettingsController *settingsController;
    ProfileController *profileController;
    ViewGameController *viewGameController;

    RedisManager *red;
    User user;
    GameGraphicOptions configuration;

    void addButtonEvents();

    void showGamemode(wxCommandEvent &);
    void showSettings(wxCommandEvent &);
    void showProfile(wxCommandEvent &);
    void showViewGame(wxCommandEvent &);
    void Exit(wxCommandEvent &);
};

#endif // HOMECONTROLLER_H
