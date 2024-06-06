#include "GamemodeController.h"

#include <hiredis/hiredis.h>
#include <unistd.h>
#include <thread>

GamemodeController::GamemodeController(wxPanel* parent)
{
    panel = new GameMode(parent->GetParent());
    panel->Hide();
    backPanel = parent;
    addButtonEvents();

    red = new RedisManager();
}

void GamemodeController::addButtonEvents() {
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &GamemodeController::BackPanel, this);

    panel->GetBtnComputer()->Bind(wxEVT_BUTTON,  [this](wxCommandEvent& event){
        ChangePanel(panel->GetComputerPanel(),panel->GetMultiplayerPanel());
    });
    panel->GetBtnMultiplayer()->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        ChangePanel(panel->GetMultiplayerPanel(), panel->GetComputerPanel());
    });
    panel->GetBtnStartMultiplayer()->Bind(wxEVT_BUTTON, &GamemodeController::SearchOpponent, this);
    panel->GetBtnStartComputer()->Bind(wxEVT_BUTTON, &GamemodeController::StartGame, this);
}

void WaitResponse(redisContext *c) {
    // std::cout << "Waiting for response" << std::endl;
    // while (true) {
    //     redisReply *reply = nullptr;
    //     if (redisGetReply(c, (void**)&reply) == REDIS_OK && reply != nullptr) {
    //         if (reply != NULL) {
    //             std::cout << "Received type: " << reply->type << std::endl;
    //             std::cout << "Received message: " << reply->element[2]->str << std::endl;
    //         }
    //         else {
    //             std::cerr << "Failed to receive message" << std::endl;
    //         }
    //         freeReplyObject(reply);
    //         break;
    //     }
    //     std::cout << "non entra nell'if" << std::endl;
    // }
    // std::cout << "Quitting" << std::endl;
}

void GamemodeController::SearchOpponent(wxCommandEvent& event) {
    if (searching)
        return;

    std::cout << "Searching for opponent" << std::endl;

    GameOptions* options = panel->GetGameOptions();
    int seconds = options->GetGameDurationInSeconds();
    int increment = options->GetGameIncrement();
    std::string mode = std::to_string(seconds) + "+" + std::to_string(increment);

    // Connect to server and send game request
    if (!red->Connect()) {
        std::cerr << "Failed to connect to Redis server" << std::endl;
        return;
    }

    const char* channel = "new_clients";
    if (!red->CheckChannel(channel)) {
        std::cerr << "Failed to check new_clients channel" << std::endl;
        return;
    }

    // Generate a unique client ID (replace this with your own logic)
    std::string client_id = "client_" + std::to_string(getpid());
    std::string message = client_id + ":" + mode;

    // Publish client ID to 'new_clients' channel
    if (!red->PublishToChannel(channel, message.c_str())) {
        std::cerr << "Failed to publish message to " << channel << " channel" << std::endl;
        return;
    }

    // Set this to true so that the user can't search for another opponent
    searching = true;

    // wait asynchronously for the server to find an opponent
    if (!red->SubscribeToChannel(client_id.c_str())) {
        std::cerr << "Failed to subscribe to " << client_id << " channel" << std::endl;
        return;
    }

    auto f = [this, client_id]() {
        std::string response = red->WaitResponse();
        searching = false;

        // Parse the response
        std::string delimiter = ":";
        size_t pos = response.find(delimiter);
        std::string opponent_id = response.substr(0, pos);
        int side = std::stoi(response.substr(pos + 1)) + 1;
        std::cout << "Found opponent: " << opponent_id << " with side " << side << std::endl;

        if (!red->UnsubscribeFromChannel(client_id.c_str())) {
            return;
        }
        
        panel->CallAfter([this, opponent_id, side]() {
            GameOptions* options = panel->GetGameOptions();
            options->SetStartSide(side);
            std::cout << "Passing to gameplay controller " << opponent_id << std::endl;
            gameplayController = new GameplayController(options, red, opponent_id);
        });
    };

    std::thread t(f);
    t.detach();
}

void GamemodeController::StartGame(wxCommandEvent& event){
    GameOptions* options = panel->GetGameOptions();
//    printf("Against human is %d \n",options->GetAgaintsHuman());
//    printf("Gametime is %d|%d \n",options->GetGameDurationInSeconds(),options->GetGameIncrement());
//    printf("Computer elo is %d \n",options->GetComputerElo());
//    printf("You start with %d \n",options->GetStartSide());
//    printf("---------------------------------------------------\n");
    gameplayController = new GameplayController(options);

//    if (gameplayController == nullptr) {
//        gameplayController = new GameplayController(options);
//    }
//    else {
//        // gameplayController.startaGame ?
//    }
}


void GamemodeController::ChangePanel(wxPanel* show, wxPanel* hide) {
    show->Show();
    hide->Hide();
}

void GamemodeController::ShowPanel() {
    panel->Show();
}

void GamemodeController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->GetComputerPanel()->Hide();
    panel->GetMultiplayerPanel()->Hide();
    panel->Hide();
}

GamemodeController::~GamemodeController()
{

}