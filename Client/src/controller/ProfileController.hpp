#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include "../lib.hpp"
#include "../view/ProfilePanel.hpp"
#include "../model/User.hpp"
#include "RedisManager.hpp"
#include "JsonManager.hpp"

class ProfileController
{
public:
    ProfileController(wxPanel *, RedisManager *, User &);
    ~ProfileController();

    void ShowPanel();
    void UpdateProfile(User &);

private:
    ProfilePanel *panel;
    wxPanel *backPanel;
    void addButtonEvents();
    void BackPanel(wxCommandEvent &);
    void SaveUser(wxCommandEvent &);
    void ShowDefault(wxCommandEvent &);
    void ShowUpdate(wxCommandEvent &);

    User *user;
    RedisManager *red;
};

#endif // PROFILECONTROLLER_
