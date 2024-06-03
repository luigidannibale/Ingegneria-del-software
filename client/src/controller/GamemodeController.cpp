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
    std::cout << "Waiting for response" << std::endl;
    while (true) {
        redisReply *reply = nullptr;
        if (redisGetReply(c, (void**)&reply) == REDIS_OK && reply != nullptr) {
            if (reply != NULL) {
                std::cout << "Received type: " << reply->type << std::endl;
                std::cout << "Received message: " << reply->element[2]->str << std::endl;
            }
            else {
                std::cerr << "Failed to receive message" << std::endl;
            }
            freeReplyObject(reply);
            break;
        }
        std::cout << "non entra nell'if" << std::endl;
    }
    std::cout << "Quitting" << std::endl;
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
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            std::cerr << "Connection error: " << c->errstr << std::endl;
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        return;
    }

    // Check if someone is on the 'new_clients' channel
    redisReply *reply = (redisReply*)redisCommand(c, "PUBSUB NUMSUB new_clients");
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY || reply->elements != 2) {
        std::cerr << "Failed to check new_clients channel" << std::endl;
        freeReplyObject(reply);
        return;
    }

    int numSubscribers = reply->element[1]->integer;
    freeReplyObject(reply);;

    if (numSubscribers < 0) {
        std::cout << "No one is on the 'new_clients' channel" << std::endl;
        return;
    }

    // Generate a unique client ID (replace this with your own logic)
    std::string client_id = "client_" + std::to_string(getpid());
    std::string message = client_id + ":" + mode;

    // Publish client ID to 'new_clients' channel
    reply = (redisReply*)redisCommand(c, "PUBLISH new_clients %s", message.c_str());
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
        std::cerr << "Failed to publish client ID" << std::endl;
        std::cout << "Errore: " << reply->str << std::endl;
        freeReplyObject(reply);
        return;
    }

    // Set this to true so that the user can't search for another opponent
    searching = true;

    // wait asynchroneously for the server to find an opponent
    reply = (redisReply*)redisCommand(c, "SUBSCRIBE %s", client_id.c_str());
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Failed to subscribe to new_clients channel" << std::endl;
        freeReplyObject(reply);
        return;
    }
    std::cout << "Subscribed to " << client_id.c_str() << " channel" << std::endl;
    freeReplyObject(reply);

    std::thread t(WaitResponse, c);
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