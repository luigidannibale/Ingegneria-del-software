#include "Server.hpp"


Server::Server() {
    this->stop_server = false;
    this->redisManager = new RedisManager();

    redisManager->Connect();
    redisManager->SubscribeToChannel(NEW_CLIENTS_CHANNEL.c_str());

    listen_for_clients();

}

void Server::handle_client(const std::string) {

}
void Server::listen_for_clients() {
    while(!stop_server){
        std::string message = redisManager->WaitResponse();
        if (message.empty()) continue;
        std::thread(&Server::handle_client, this, message).detach();
    }
}