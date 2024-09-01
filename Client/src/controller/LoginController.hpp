#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include "../view/LoginPanel.hpp"
#include "RedisManager.hpp"
#include "JsonManager.hpp"
#include "../model/User.hpp"
#include "../view/GameGraphicOptions.hpp"

class LoginController
{
public:
    LoginController(wxPanel *, RedisManager *, User &, GameGraphicOptions &);
    ~LoginController();

    void ShowPanel();

private:
    LoginPanel *panel;
    wxPanel *backPanel;

    RedisManager *red;
    User *user;
    GameGraphicOptions *configuration;

    void addButtonEvents();
    void Login(wxCommandEvent &event);
};

#endif // LOGINCONTROLLER_H