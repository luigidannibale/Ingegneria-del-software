#include "Server.hpp"


Server::Server() {
    this->stop_server = false;
    // this->redisManager = new RedisManager();

    //Redis connection part
    /*redisManager->Connect();
    bool accepted = redisManager->SubscribeToChannel(NEW_CLIENTS_CHANNEL.c_str());
    std::string s = accepted ? "Connection accepted" : "Connection not accepted";
    std::cout<< s << std::endl;
    listen_for_clients();*/

    Database DB = Database();
}


void Server::handle_client(std::string messaggio) {
    json jmess = json::parse(messaggio);
    Richiesta r = Richiesta::fromJson(jmess);
    switch (r.getCodice()) {
        case CodiceRichiesta::new_game:
            new_game(r.getPayload().at("time_duration"),
                r.getPayload().at("time_increment"),
                r.getPayload().at("black"),
                r.getPayload().at("white"));
            break;
        case CodiceRichiesta::search_opponent:
            search_opponent( r.getPayload().at("user"),
                             r.getPayload().at("time_duration"),
                             r.getPayload().at("time_increment"));
            break;
        case CodiceRichiesta::update_game:
            update_game(r.getPayload().at("game_id"),
                        r.getPayload().at("moves"),
                        r.getPayload().at("esito"),
                        r.getPayload().at("motivo"));
            break;
        case CodiceRichiesta::search_game:
            search_game(r.getPayload().at("game_id"));
            break;
        case CodiceRichiesta::new_user:
            new_user(r.getPayload().at("username"),
                     r.getPayload().at("nome"),
                     r.getPayload().at("cognome"),
                     r.getPayload().at("elo"),
                     r.getPayload().at("chessboard_style"),
                     r.getPayload().at("pieces_style"));
            break;
        case CodiceRichiesta::update_user:
            update_user(r.getPayload().at("username"),
                     r.getPayload().at("nome"),
                     r.getPayload().at("cognome"),
                     r.getPayload().at("elo"));
            break;
        case CodiceRichiesta::update_userPreference:
            update_userPreference(r.getPayload().at("username"),
                     r.getPayload().at("chessboard_style"),
                     r.getPayload().at("pieces_style"));
            break;
        case CodiceRichiesta::delete_user:
            delete_user(r.getPayload().at("username"));
            break;
    }
}

void Server::listen_for_clients() {
    while(!stop_server){
        std::string message= redisManager->WaitResponse();
        if (message.empty()) continue;
        std::thread(&Server::handle_client, this, message).detach();
    }
}

json Server::new_game(int duration, int increment, std::string u_id_b, std::string u_id_w) {
    return json();
}

json Server::search_opponent(std::string u_id, int duration, int increment) {
    return json();
}

json Server::update_game(int g_id, std::vector<std::string> moves, Esito e, Motivo m) {
    return json();
}

json Server::search_game(int g_id) {
    return json();
}

json Server::new_user(std::string username, std::string nome, std::string cognome, int elo, Chessboard_style c_st,
                      Pieces_style p_st) {
    return json();
}

json Server::update_user(std::string username, std::string nome, std::string cognome, int elo) {
    return json();
}

json Server::update_userPreference(std::string username, Chessboard_style c_st, Pieces_style p_st) {
    return json();
}

json Server::delete_user(std::string username) {
    return json();
}

int main() {
    Server server = Server();
    return 0;
}