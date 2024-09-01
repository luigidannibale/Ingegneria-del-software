#include "SettingsController.hpp"

SettingsController::SettingsController(wxPanel *parent, RedisManager *redisManager, GameGraphicOptions &configuration)
{
    // configuration = new GameGraphicOptions(BoardStyle::brown, PiecesStyle::neo);
    this->redisManager = redisManager;
    this->configuration = &configuration;
    GameGraphicOptions *viewConfiguration = new GameGraphicOptions(this->configuration->GetBoardStyle(), this->configuration->GetPiecesStyle());
    panel = new SettingsPanel(parent->GetParent(), viewConfiguration);
    panel->Hide();
    backPanel = parent;
    addButtonEvents();
}

void SettingsController::addButtonEvents()
{
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &SettingsController::BackPanel, this);
    panel->GetBtnSave()->Bind(wxEVT_BUTTON, &SettingsController::SaveSettings, this);
}

void SettingsController::ShowPanel()
{
    panel->Show();
}

void SettingsController::SetGameGraphicOptions(GameGraphicOptions *configuration)
{
    // this->configuration = configuration;
    panel->SetGameGraphicOptions(configuration);
}

void SettingsController::SetUsername(std::string username)
{
    this->username = username;
}

void SettingsController::BackPanel(wxCommandEvent &event)
{
    backPanel->Show();
    panel->Hide();
    panel->SetGameGraphicOptions(configuration);
}

void SettingsController::SaveSettings(wxCommandEvent &event)
{
    auto new_configuration = panel->GetGameGraphicOptions();

    Messaggio messaggio;
    messaggio.codice = static_cast<int>(CodiceRichiesta::update_userPreference);
    messaggio.input["username"] = username;
    messaggio.input["chessboard_style"] = new_configuration->GetBoardStyle();
    messaggio.input["pieces_style"] = new_configuration->GetPiecesStyle();

    json j = messaggio;

    if (!redisManager->PublishToChannel(redisManager->SERVER_CHANNEL, j.dump().c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    if (!redisManager->SubscribeToChannel(username.c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    auto f = [this, new_configuration]()
    {
        std::string response = redisManager->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (json::exception &e)
        {
            wxLogMessage("Error saving settings");
        }

        if (risposta.codice != static_cast<int>(CodiceRisposta::ok))
        {
            wxLogMessage("Error saving settings");
            return;
        }

        configuration->SetBoardStyle(new_configuration->GetBoardStyle());
        configuration->SetPiecesStyle(new_configuration->GetPiecesStyle());

        panel->CallAfter([this]()
                         {
            backPanel->Show();
            panel->Hide();
            panel->SetGameGraphicOptions(configuration); });
    };

    std::thread t(f);
    t.detach();
}

GameGraphicOptions *SettingsController::GetGameGraphicOptions()
{
    return configuration;
}

SettingsController::~SettingsController()
{
}