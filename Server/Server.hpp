#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include "RedisManager.hpp"

class Server {
    Server();

private:
    void handle_client(const std::string);
    void listen_for_clients();

    std::vector<std::string> connected_clients;
    std::unordered_map<std::string, std::vector<std::string>> client_map;
    bool stop_server;
    RedisManager* redisManager;
    const std::string NEW_CLIENTS_CHANNEL = "newclients";
};


#endif //SERVER_SERVER_HPP
