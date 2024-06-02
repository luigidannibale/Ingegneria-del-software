#include "GamemodeController.h"


#include <hiredis/hiredis.h>
#include <unistd.h>

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

void GamemodeController::SearchOpponent(wxCommandEvent& event) {
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
    std::cout << " Qui ci arriva " << reply->element[1] << std::endl;
    std::cout << " Qui ci arriva " << reply->element[1]->integer << std::endl;
    int numSubscribers = reply->element[1]->integer;
    std::cout << " Qui non ci arriva " << std::endl;
    freeReplyObject(reply);;

    if (numSubscribers > 0) {
        std::cout << "Someone is on the 'new_clients' channel" << std::endl;
    } else {
        std::cout << "No one is on the 'new_clients' channel" << std::endl;
        return;
    }

    // Generate a unique client ID (replace this with your own logic)
    std::string client_id = "client_" + std::to_string(getpid()) + ":" + mode;

    // Publish client ID to 'new_clients' channel
    reply = (redisReply*)redisCommand(c, "PUBLISH new_clients %s", client_id.c_str());
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
        std::cerr << "Failed to publish client ID" << std::endl;
        std::cout << "Errore: " << reply->str << std::endl;
        freeReplyObject(reply);
        return;
    }

    // wait asynchroneously for the server to find an opponent
    reply = (redisReply*)redisCommand(c, "SUBSCRIBE %s", client_id.c_str());
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Failed to subscribe to new_clients channel" << std::endl;
        freeReplyObject(reply);
        return;
    }
    std::cout << "Subscribed to " << client_id.c_str() << " channel" << std::endl;
    freeReplyObject(reply);
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