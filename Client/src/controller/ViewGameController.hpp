#ifndef VIEWGAMECONTROLLER_H
#define VIEWGAMECONTROLLER_H

#include "../view/ViewGamePanel.hpp"
#include "RedisManager.hpp"
#include "JsonManager.hpp"
#include "GameplayController.hpp"
#include "../view/GameGraphicOptions.hpp"
#include <iostream>
#include <vector>

class ViewGameController
{
public:
    ViewGameController(wxPanel *backPanel, RedisManager *red);
    ~ViewGameController();

    void ShowPanel(std::string username);
    void SetGameGraphicOptions(GameGraphicOptions *options);

private:
    RedisManager *red;
    ViewGamePanel *panel;
    wxPanel *backPanel;
    std::string username;

    GameGraphicOptions *gameGraphicOptions;

    void addButtonEvents();
    void OnCellClick(wxGridEvent &event);
    void Back(wxCommandEvent &event);
};

#endif // VIEWGAMECONTROLLER_H