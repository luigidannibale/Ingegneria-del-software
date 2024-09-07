#ifndef GAMEMODECONTROLLER_H
#define GAMEMODECONTROLLER_H

#include <cstdio>
#include "../lib.hpp"
#include "../view/GameMode.hpp"
#include "../view/LoginPanel.hpp"
#include "../model/User.hpp"
#include "GameplayController.hpp"
#include "RedisManager.hpp"
#include "JsonManager.hpp"

class GamemodeController
{
public:
    GamemodeController(wxPanel *, RedisManager *, User &);
    ~GamemodeController();

    void ShowPanel();
    void SetGameGraphicOptions(GameGraphicOptions *);

private:
    GameMode *panel;
    wxPanel *backPanel;
    GameGraphicOptions *configuration;

    bool searching = false;
    std::string message;
    GameplayController *gameplayController;
    RedisManager *red;
    User *user;

    Ricerca ricerca;

    void addButtonEvents();
    void SearchOpponent(wxCommandEvent &);
    void StopSearchOpponent(wxCommandEvent &);
    void StartGame(wxCommandEvent &);
    void BackPanel(wxCommandEvent &);
    void ChangePanel(wxPanel *, wxPanel *);
};

#endif // GAMEMODECONTROLLER_H
