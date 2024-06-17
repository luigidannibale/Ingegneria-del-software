#include <iostream>
#include <thread>
#include <hiredis/hiredis.h>
#include <cstring>
#include <vector>
#include <signal.h>
#include <unordered_map>
#include "Database.h"

std::vector<std::string> connected_clients;
std::unordered_map<std::string, std::vector<std::string>> client_map;

bool stop_server = false;

void subscribe(redisContext *c) {
    redisReply *reply = (redisReply*)redisCommand(c, "SUBSCRIBE new_clients");
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Failed to subscribe to new_clients channel" << std::endl;
        freeReplyObject(reply);
        return;
    }
    std::cout << "Subscribed to new_clients channel" << std::endl;
    freeReplyObject(reply);
}

void unsubscribe(redisContext *c) {
    redisReply *reply = (redisReply*)redisCommand(c, "UNSUBSCRIBE new_clients");
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        std::cerr << "Failed to unsubscribe from new_clients channel" << std::endl;
        freeReplyObject(reply);
        return;
    }
    std::cout << "Unsubscribed from new_clients channel" << std::endl;
    freeReplyObject(reply);
}

void handle_client(redisContext *c, const std::string& client_id) {
    // Add client to waiting list
    std::string delimiter = ":";
    size_t pos = client_id.find(delimiter);
    std::string key = client_id.substr(pos + delimiter.length());
    std::string value = client_id.substr(0, pos);
    std::cout << "Value " << value << " Key: " << key << std::endl;

    if (value == "quit") {
        pos = key.find(delimiter);
        value = key.substr(0, pos);
        key = key.substr(pos + delimiter.length());

        auto it = std::find(client_map[key].begin(), client_map[key].end(), value);
        if (it != client_map[key].end()) {
            client_map[key].erase(it);
            std::cout << "Client successfully removed from waiting list" << std::endl;
            return;
        }
        std::cout << "Client not found in waiting list" << std::endl;
        return;
    }
    
    client_map[key].push_back(value);

    if (client_map[key].size() >= 2) { // join two players
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string player1 = client_map[key].back();
        client_map[key].pop_back();
        std::string player2 = client_map[key].back();
        client_map[key].pop_back();

        unsubscribe(c);

        // Choose who to start the game
        int random = rand() % 2;

        std::string reply1 = player1 + ":" + std::to_string(random);
        std::string reply2 = player1 + ":" + std::to_string(!random);

        // Notify players that they have been matched
        // std::cout << "Prima del comando 1" << std::endl;
        redisReply *reply = (redisReply*)redisCommand(c, "PUBLISH %s %s", player1.c_str(), reply1.c_str());
        std::cout << "Dopo del comando 1" << std::endl;
        if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
            if (reply != NULL && reply->str != NULL) {
                std::cerr << reply->str << std::endl;
                std::cerr << reply->type << std::endl;
            } else {
                std::cerr << "Unknown error" << std::endl;
            }
            freeReplyObject(reply);
            return;
        }
        freeReplyObject(reply);

        // std::cout << "Prima del comando 2" << std::endl;
        reply = (redisReply*)redisCommand(c, "PUBLISH %s %s", player2.c_str(), reply2.c_str());
        std::cout << "Dopo del comando 2" << std::endl;
        if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
            if (reply != NULL && reply->str != NULL) {
                std::cerr << reply->str << std::endl;
                std::cerr << reply->type << std::endl;
            } else {
                std::cerr << "Unknown error" << std::endl;
            }
            freeReplyObject(reply);
            return;
        }
        
        freeReplyObject(reply);

        std::cout << "Matched players: " << player1 << " and " << player2 << std::endl;
        subscribe(c);
    }
}

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "Received termination signal. Shutting down..." << std::endl;
        stop_server = true;
    }
}

void listen_for_clients(redisContext *c) {
    subscribe(c);

    while (!stop_server) {
        redisReply *reply = nullptr;
        if (redisGetReply(c, (void**)&reply) == REDIS_OK && reply != nullptr) {
            if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 &&
                strcmp(reply->element[0]->str, "message") == 0 &&
                strcmp(reply->element[1]->str, "new_clients") == 0) {
                std::string client_id(reply->element[2]->str);
                std::thread(handle_client, c, client_id).detach();
            }
            freeReplyObject(reply);
        }
    }

    unsubscribe(c);
    std::cout << "Shutting down..." << std::endl;
}

int main() {
    // signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);

    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            std::cerr << "Connection error: " << c->errstr << std::endl;
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        redisFree(c);
        return 1;
    }

    listen_for_clients(c);
    std::cout << "Dopo listen_for_clients" << std::endl;
    // std::thread listen_thread(listen_for_clients, c);
    // listen_thread.join();

    redisFree(c);

//    try {
        ;
        PGconn *conn = PQconnectdb(connection_string);

        // Check to see that the backend connection was successfully made
        if (PQstatus(conn) != CONNECTION_OK) {
            std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
            PQfinish(conn);
            return 1;
        }

        createSchema(conn);
        std::cout << "Database schema created successfully." << std::endl;

        PQfinish(conn);
        //db.insertGiocatore("john_doe", "John", "Doe", "123", 1500);
//        db.insertGiocatore("jane_smith", "Jane", "Smith", "123", 1600);
//        db.insertGiocatore("alice_jones", "Alice", "Jones", "123", 1700);
//        db.insertGiocatore("bob_brown", "Bob", "Brown", "123", 1800);
//        db.insertGiocatore("charlie_black", "Charlie", "Black", "123", 1900);
//
//        db.insertPreferenza("john_doe", 1);
//        db.insertPreferenza("jane_smith", 2);
//        db.insertPreferenza("alice_jones", 3);
//        db.insertPreferenza("bob_brown", 4);
//        db.insertPreferenza("charlie_black", 5);
//
//        db.insertTema(1, 1);
//        db.insertTema(1, 2);
//        db.insertTema(2, 1);
//        db.insertTema(2, 2);
//        db.insertTema(3, 1);
//        db.insertTema(3, 2);

//
//
//    } catch (const std::exception &e) {
//        std::cerr << e.what() << std::endl;
//    }

    return 0;
}
