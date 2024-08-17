#ifndef GAMEMODECONTROLLER_H
#define GAMEMODECONTROLLER_H

#include <cstdio>
#include "../lib.hpp"
#include "../view/GameMode.hpp"
#include "../view/LoginPanel.hpp"
#include "GameplayController.hpp"
#include "RedisManager.hpp"

class GamemodeController
{
public:
    GamemodeController(wxPanel *);
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

    void addButtonEvents();
    void SearchOpponent(wxCommandEvent &);
    void StopSearchOpponent(wxCommandEvent &);
    void StartGame(wxCommandEvent &);
    void BackPanel(wxCommandEvent &);
    void ChangePanel(wxPanel *, wxPanel *);
};

#endif // GAMEMODECONTROLLER_H
