#include "GamemodeController.hpp"

#include <hiredis/hiredis.h>
#include <unistd.h>
#include <thread>

GamemodeController::GamemodeController(wxPanel *parent, RedisManager *red, User &user)
{
    panel = new GameMode(parent);
    panel->Hide();
    backPanel = parent;
    addButtonEvents();

    this->red = red;
    this->user = &user;
}

void GamemodeController::addButtonEvents()
{
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &GamemodeController::BackPanel, this);

    panel->GetBtnComputer()->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                                  { ChangePanel(panel->GetComputerPanel(), panel->GetMultiplayerPanel()); });
    panel->GetBtnMultiplayer()->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                                     { ChangePanel(panel->GetMultiplayerPanel(), panel->GetComputerPanel()); });
    panel->GetBtnStartMultiplayer()->Bind(wxEVT_BUTTON, &GamemodeController::SearchOpponent, this);
    panel->GetBtnStartComputer()->Bind(wxEVT_BUTTON, &GamemodeController::StartGame, this);
    panel->GetBtnQuitMultiplayer()->Bind(wxEVT_BUTTON, &GamemodeController::StopSearchOpponent, this);
}

void GamemodeController::SearchOpponent(wxCommandEvent &event)
{
    if (searching)
        return;

    std::cout << "Searching for opponent" << std::endl;

    GameOptions *options = panel->GetGameOptions();

    int seconds = options->GetGameDurationInSeconds();
    int increment = options->GetGameIncrement();
    std::string mode = std::to_string(seconds) + "+" + std::to_string(increment);

    // Connect to server and send game request
    // if (!red->Connect())
    // {
    //     std::cerr << "Failed to connect to Redis server" << std::endl;
    //     return;
    // }

    // // const char *channel = "new_clients";
    // if (!red->CheckChannel(red->SERVER_CHANNEL))
    // {
    //     std::cerr << "Failed to check new_clients channel" << std::endl;
    //     return;
    // }

    ricerca.user = user->Username();
    ricerca.time_duration = seconds;
    ricerca.time_increment = increment;

    Messaggio richiesta;
    richiesta.codice = static_cast<int>(CodiceRichiesta::search_opponent);
    richiesta.input = json(ricerca);
    json j = richiesta;

    // Publish client ID to 'new_clients' channel
    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        std::cerr << "Failed to publish message to " << red->SERVER_CHANNEL << " channel" << std::endl;
        return;
    }

    // Set this to true so that the user can't search for another opponent
    searching = true;
    panel->GetBtnQuitMultiplayer()->Show();
    panel->GetBtnBack()->Disable();

    // wait asynchronously for the server to find an opponent
    if (!red->SubscribeToChannel(ricerca.user.c_str()))
    {
        std::cerr << "Failed to subscribe to " << ricerca.user << " channel" << std::endl;
        return;
    }

    std::string user_id = ricerca.user;
    auto f = [this, user_id]()
    {
        // Wait for ok from the server
        std::string response = red->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (json::parse_error &e)
        {
            wxLogMessage("Failed to parse response");
            red->UnsubscribeFromChannel();
            panel->CallAfter([this]()
                             {
                searching = false;
                panel->GetBtnQuitMultiplayer()->Hide();
                panel->GetBtnBack()->Enable(); });
            return;
        }
        if (risposta.codice != static_cast<int>(CodiceRisposta::ok))
        {
            wxLogMessage("Failed to find opponent");
            red->UnsubscribeFromChannel();
            panel->CallAfter([this]()
                             {
                searching = false;
                panel->GetBtnQuitMultiplayer()->Hide();
                panel->GetBtnBack()->Enable(); });
            return;
        }

        // Now wait for an opponent to be found
        response = red->WaitResponse(false);
        try
        {
            risposta = json::parse(response);
        }
        catch (json::parse_error &e)
        {
            wxLogMessage("Failed to parse response");
            red->UnsubscribeFromChannel();
            panel->CallAfter([this]()
                             {
                searching = false;
                panel->GetBtnQuitMultiplayer()->Hide();
                panel->GetBtnBack()->Enable(); });
            return;
        }
        searching = false;
        if (risposta.codice != static_cast<int>(CodiceRisposta::game_created))
        {
            wxLogMessage("Failed to find opponent");
            red->UnsubscribeFromChannel();
            panel->CallAfter([this]()
                             {
                panel->GetBtnQuitMultiplayer()->Hide();
                panel->GetBtnBack()->Enable(); });
            return;
        }

        std::string opponent_id = risposta.input["opponent"];
        int side = risposta.input["side"];
        side += 1;
        int gameId = risposta.input["game_id"];

        // Parse the response
        // std::string delimiter = ":";
        // size_t pos = response.find(delimiter);
        // std::string opponent_id = response.substr(0, pos);
        // int side = std::stoi(response.substr(pos + 1)) + 1;
        std::cout << "Found opponent: " << opponent_id << " with side " << side << " of game " << gameId << std::endl;

        // if (!red->UnsubscribeFromChannel(ricerca.user.c_str()))
        // {
        //     return;
        // }

        panel->CallAfter([this, user_id, opponent_id, side, gameId]()
                         {
            panel->GetBtnQuitMultiplayer()->Hide();
            panel->GetBtnBack()->Enable();
            panel->GetParent()->Hide();
            GameOptions* options = panel->GetGameOptions();
            // GameGraphicOptions* graphicOptions = panel->GetGameGraphicOptions();
            options->SetStartSide(side);
            std::cout << "Passing to gameplay controller " << opponent_id << std::endl;
            gameplayController = new GameplayController(panel, options, configuration, red, user_id, opponent_id, gameId); });
    };

    std::thread t(f);
    t.detach();
}

void GamemodeController::StopSearchOpponent(wxCommandEvent &event)
{
    std::cout << "Quit search multiplayer" << std::endl;

    // if (!red->UnsubscribeFromChannel())
    // {
    //     std::cerr << "Failed to unsubscribe from channel" << std::endl;
    //     return;
    // }

    Messaggio richiesta;
    richiesta.codice = static_cast<int>(CodiceRichiesta::quit_search_opponent);
    richiesta.input = json(ricerca);
    json j = richiesta;
    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        wxLogMessage("Failed to contact the server");
        return;
    }

    auto f = [this]()
    {
        std::string response = red->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (json::parse_error &e)
        {
            wxLogMessage("Failed to parse response");
            return;
        }
        if (risposta.codice != static_cast<int>(CodiceRisposta::ok))
        {
            wxLogMessage("Failed to quit search");
            return;
        }
        panel->CallAfter([this]()
                         {
            searching = false;
            panel->GetBtnQuitMultiplayer()->Hide();
            panel->GetBtnBack()->Enable(); });
        red->UnsubscribeFromChannel();
    };

    std::thread t(f);
    t.detach();
}

void GamemodeController::StartGame(wxCommandEvent &event)
{
    GameOptions *options = panel->GetGameOptions();
    // GameGraphicOptions* graphicOptions = panel->GetGameGraphicOptions();
    //    printf("Against human is %d \n",options->GetAgaintsHuman());
    //    printf("Gametime is %d|%d \n",options->GetGameDurationInSeconds(),options->GetGameIncrement());
    //    printf("Computer elo is %d \n",options->GetComputerElo());
    //    printf("You start with %d \n",options->GetStartSide());
    //    printf("---------------------------------------------------\n");
    panel->GetParent()->Hide();

    gameplayController = new GameplayController(panel, options, configuration);

    //    if (gameplayController == nullptr) {
    //        gameplayController = new GameplayController(options);
    //    }
    //    else {
    //        // gameplayController.startaGame ?
    //    }
}

void GamemodeController::ChangePanel(wxPanel *show, wxPanel *hide)
{
    show->Show();
    hide->Hide();
}

void GamemodeController::ShowPanel()
{
    panel->Show();
}

void GamemodeController::SetGameGraphicOptions(GameGraphicOptions *configuration)
{
    this->configuration = configuration;
}

void GamemodeController::BackPanel(wxCommandEvent &event)
{
    backPanel->Show();
    panel->GetComputerPanel()->Hide();
    panel->GetMultiplayerPanel()->Hide();
    panel->Hide();
}

GamemodeController::~GamemodeController()
{
}