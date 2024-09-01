#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "../lib.hpp"
#include "../view/SettingsPanel.hpp"
#include "../view/GameGraphicOptions.hpp"
#include "JsonManager.hpp"
#include "RedisManager.hpp"

class SettingsController
{
public:
    SettingsController(wxPanel *, RedisManager *, GameGraphicOptions &);
    ~SettingsController();

    void ShowPanel();
    void SetGameGraphicOptions(GameGraphicOptions *);
    void SetUsername(std::string);
    GameGraphicOptions *GetGameGraphicOptions();

private:
    SettingsPanel *panel;
    wxPanel *backPanel;

    RedisManager *redisManager;
    GameGraphicOptions *configuration;
    std::string username;

    void addButtonEvents();
    void BackPanel(wxCommandEvent &);
    void SaveSettings(wxCommandEvent &);
};

#endif // SETTINGSCONTROLLER_H
