#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include "../view/LoginPanel.hpp"
#include "RedisManager.hpp"

class LoginController
{
public:
    LoginController(wxPanel *, RedisManager *);
    ~LoginController();

    void ShowPanel();

private:
    LoginPanel *panel;
    wxPanel *backPanel;

    RedisManager* red;

    void addButtonEvents();
    void Login(wxCommandEvent &event);
};

#endif // LOGINCONTROLLER_H