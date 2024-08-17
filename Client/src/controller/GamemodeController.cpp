#include "GamemodeController.hpp"

#include <hiredis/hiredis.h>
#include <unistd.h>
#include <thread>

namespace ns
{
    void from_json(const json &j, Richiesta &r)
    {
        r.codice = j.at("codice").get<int>();
        r.input = j.at("input").get<json>();
    }

    void to_json(json &j, const Richiesta &r)
    {
        j = json{{"codice", r.codice}, {"input", r.input}};
    }

    void from_json(const json &j, Ricerca &r)
    {
        r.user = j.at("user").get<std::string>();
        r.time_duration = j.at("time_duration").get<int>();
        r.time_increment = j.at("time_increment").get<int>();
    }

    void to_json(json &j, const Ricerca &r)
    {
        j = json{{"user", r.user}, {"time_duration", r.time_duration}, {"time_increment", r.time_increment}};
    }
}

GamemodeController::GamemodeController(wxPanel *parent)
{
    panel = new GameMode(parent->GetParent());
    panel->Hide();
    backPanel = parent;
    addButtonEvents();

    red = new RedisManager();
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
    if (!red->Connect())
    {
        std::cerr << "Failed to connect to Redis server" << std::endl;
        return;
    }

    const char *channel = "new_clients";
    if (!red->CheckChannel(channel))
    {
        std::cerr << "Failed to check new_clients channel" << std::endl;
        return;
    }

    ricerca.user = "client_" + std::to_string(getpid());
    ricerca.time_duration = seconds;
    ricerca.time_increment = increment;

    ns::Richiesta richiesta;
    richiesta.codice = static_cast<int>(ns::CodiceRichiesta::search_opponent);
    richiesta.input = json(ricerca);
    json j = richiesta;

    // Publish client ID to 'new_clients' channel
    if (!red->PublishToChannel(channel, j.dump().c_str()))
    {
        std::cerr << "Failed to publish message to " << channel << " channel" << std::endl;
        return;
    }

    // Set this to true so that the user can't search for another opponent
    searching = true;
    panel->GetBtnQuitMultiplayer()->Show();

    // wait asynchronously for the server to find an opponent
    if (!red->SubscribeToChannel(ricerca.user.c_str()))
    {
        std::cerr << "Failed to subscribe to " << ricerca.user << " channel" << std::endl;
        return;
    }

    std::string user_id = ricerca.user;
    auto f = [this, user_id]()
    {
        std::string response = red->WaitResponse();
        searching = false;

        std::cout << "Received response: " << response << std::endl;

        // Parse the response
        std::string delimiter = ":";
        size_t pos = response.find(delimiter);
        std::string opponent_id = response.substr(0, pos);
        int side = std::stoi(response.substr(pos + 1)) + 1;
        std::cout << "Found opponent: " << opponent_id << " with side " << side << std::endl;

        // if (!red->UnsubscribeFromChannel(ricerca.user.c_str()))
        // {
        //     return;
        // }

        panel->CallAfter([this, user_id, opponent_id, side]()
                         {
            panel->GetBtnQuitMultiplayer()->Hide();
            GameOptions* options = panel->GetGameOptions();
            // GameGraphicOptions* graphicOptions = panel->GetGameGraphicOptions();
            options->SetStartSide(side);
            std::cout << "Passing to gameplay controller " << opponent_id << std::endl;
            gameplayController = new GameplayController(options, configuration, red, user_id, opponent_id); });
    };

    std::thread t(f);
    t.detach();
}

void GamemodeController::StopSearchOpponent(wxCommandEvent &event)
{
    std::cout << "Quit search multiplayer" << std::endl;

    if (!red->UnsubscribeFromChannel())
    {
        std::cerr << "Failed to unsubscribe from channel" << std::endl;
        return;
    }

    ns::Richiesta richiesta;
    richiesta.codice = static_cast<int>(ns::CodiceRichiesta::quit_search_opponent);
    richiesta.input = json(ricerca);
    json j = richiesta;
    if (!red->PublishToChannel("new_clients", j.dump().c_str()))
    {
        std::cerr << "Failed to publish message to new_clients channel" << std::endl;
        return;
    }

    red->Disconnect();
    searching = false;
    panel->GetBtnQuitMultiplayer()->Hide();
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
    gameplayController = new GameplayController(options, configuration);

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