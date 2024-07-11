#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <string>

#include <unordered_map>
#include <thread>

#include "Database.hpp"
#include "RedisManager.hpp"



// Enum class CodiceRichiesta
enum class CodiceRichiesta {
    new_game = 101,
    search_opponent = 102,
    update_game = 103,
    search_game = 104,
    new_user = 201,
    update_user = 202,
    update_userPreference = 203,
    delete_user = 204
};

// Classe Richiesta
class Richiesta {
private:
    CodiceRichiesta codice_;
    json input_string_;

public:
    // Costruttore
    Richiesta(CodiceRichiesta codice, const json& input)
            : codice_(codice), input_string_(input) {}

    // Metodo per ottenere il codice della richiesta
    CodiceRichiesta getCodice() const {
        return codice_;
    }

    // Metodo per ottenere il payload JSON della richiesta
    const json& getPayload() const {
        return input_string_;
    }

    // Metodo per convertire la richiesta in JSON
    json toJson() const {
        return {
                {"codice", static_cast<int>(codice_)},
                {"input", input_string_}
        };
    }

    // Metodo statico per creare una richiesta da JSON
    static Richiesta fromJson(const json& j) {
        return Richiesta(static_cast<CodiceRichiesta>(j.at("codice").get<int>()), j.at("input"));
    }
};

class Server {

public:
    Server();

private:
    void handle_client(std::string);
    void listen_for_clients();

    //Operations
    json new_game(int duration,int increment,std::string u_id_b,std::string u_id_w);
    json search_opponent(std::string u_id, int duration,int increment);
    json update_game(int g_id, std::string moves, Esito e, Motivo m);
    json search_game(int g_id);
    json new_user(std::string username, std::string nome, std::string cognome, int elo, Chessboard_style c_st, Pieces_style p_st);
    json update_user(std::string username, std::string nome, std::string cognome, int elo);
    json update_userPreference(std::string username, Chessboard_style c_st, Pieces_style p_st);
    json delete_user(std::string username);

    std::unordered_map<std::string, std::vector<std::string>> connectedClients;
    bool stop_server;
    RedisManager* redisManager;
    const std::string NEW_CLIENTS_CHANNEL = "new_clients";

    Database* db;
};

#endif //SERVER_SERVER_HPP
