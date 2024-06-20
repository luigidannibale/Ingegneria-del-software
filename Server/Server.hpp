#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <tuple>
#include <nlohmann/json.hpp>
#include "RedisManager.hpp"

using json = nlohmann::json;
// Enumeration for esito
enum class Esito {
    W,  // Win
    D,  // Draw
    B,  // Banned
    NF  // Not Finished
};
// Enumeration for Motivo
enum class Motivo {
    checkmate,              // Checkmate
    wonOnTime,              // Won on Time
    quitmate,               // Quitmate
    stalemate,              // Stalemate
    insufficientMaterial,   // Insufficient Material
    threefoldRepetition,    // Threefold Repetition
    fiftyMoveRule,          // 50-Move Rule
    NF                      // Not Finished
};
// Enum for chessboard_style
enum class Chessboard_style {
    blue,
    brown,
    black
};
// Enum for pieces_style, using chessboard_style as underlying type
enum class Pieces_style{
    neo,
    pixel
};
// Serializzazione e deserializzazione per Esito
NLOHMANN_JSON_SERIALIZE_ENUM(Esito, {
    {Esito::W, "W"},
    {Esito::D, "D"},
    {Esito::B, "B"},
    {Esito::NF, "NF"}
})
// Serializzazione e deserializzazione per Motivo
NLOHMANN_JSON_SERIALIZE_ENUM(Motivo, {
    {Motivo::checkmate, "checkmate"},
    {Motivo::wonOnTime, "wonOnTime"},
    {Motivo::quitmate, "quitmate"},
    {Motivo::stalemate, "stalemate"},
    {Motivo::insufficientMaterial, "insufficientMaterial"},
    {Motivo::threefoldRepetition, "threefoldRepetition"},
    {Motivo::fiftyMoveRule, "fiftyMoveRule"},
    {Motivo::NF, "NF"}
})
// Serializzazione e deserializzazione per Chessboard_Style
NLOHMANN_JSON_SERIALIZE_ENUM(Chessboard_style, {
    {Chessboard_style::blue, "blue"},
    {Chessboard_style::brown, "brown"},
    {Chessboard_style::black, "black"}
})
// Serializzazione e deserializzazione per Pieces_style
NLOHMANN_JSON_SERIALIZE_ENUM(Pieces_style, {
    {Pieces_style::neo, "neo"},
    {Pieces_style::pixel, "pixel"}
})
class User {
public:
    // Constructor
    User(const std::string& username, const std::string& nome, const std::string& cognome, const std::string& password, int puntiElo)
            : username_(username), nome_(nome), cognome_(cognome), password_(password), puntiElo_(puntiElo) {}

    // Metodo per convertire l'oggetto User in JSON
    json toJson() const {
        return json{
                {"username", username_},
                {"nome", nome_},
                {"cognome", cognome_},
                {"password", password_},
                {"puntiElo", puntiElo_}
        };
    }

    // Metodo statico per creare un oggetto User da JSON
    static User fromJson(const json& j) {
        return User{
                j.at("username").get<std::string>(),
                j.at("nome").get<std::string>(),
                j.at("cognome").get<std::string>(),
                j.at("password").get<std::string>(),
                j.at("puntiElo").get<int>()
        };
    }

    // Getter and Setter for Username
    std::string getUsername() const { return username_; }
    void setUsername(const std::string& username) { username_ = username; }

    // Getter and Setter for Nome
    std::string getNome() const { return nome_; }
    void setNome(const std::string& nome) { nome_ = nome; }

    // Getter and Setter for Cognome
    std::string getCognome() const { return cognome_; }
    void setCognome(const std::string& cognome) { cognome_ = cognome; }

    // Getter and Setter for Password
    std::string getPassword() const { return password_; }
    void setPassword(const std::string& password) { password_ = password; }

    // Getter and Setter for PuntiElo
    int getPuntiElo() const { return puntiElo_; }
    void setPuntiElo(int puntiElo) { puntiElo_ = puntiElo; }

    // Method to display player details
    void display() const {
        std::cout << "Username: " << username_ << std::endl;
        std::cout << "Nome: " << nome_ << std::endl;
        std::cout << "Cognome: " << cognome_ << std::endl;
        std::cout << "Password: " << password_ << std::endl;
        std::cout << "Punti Elo: " << puntiElo_ << std::endl;
    }

private:
    std::string username_;
    std::string nome_;
    std::string cognome_;
    std::string password_;
    int puntiElo_;
};
class Game {
private:
    int time_duration;
    int time_increment;
    std::string u_id_b;
    std::string u_id_w;
    std::vector<std::string> moves;
    Esito esito;
    Motivo motivo;

public:
    // Constructor
    Game(int duration, int increment, const std::string& black, const std::string& white, const std::vector<std::string>& game_moves, Esito game_esito, Motivo game_motivo)
            : time_duration(duration), time_increment(increment), u_id_b(black), u_id_w(white), moves(game_moves), esito(game_esito), motivo(game_motivo) {}

    // Metodo per convertire l'oggetto Game in JSON
    json toJson() const {
        return json{
                {"time_duration", time_duration},
                {"time_increment", time_increment},
                {"u_id_b", u_id_b},
                {"u_id_w", u_id_w},
                {"moves", moves},
                {"esito", esito},
                {"motivo", motivo}
        };
    }

    // Metodo statico per creare un oggetto Game da JSON
    static Game fromJson(const json& j) {
        return Game{
                j.at("time_duration").get<int>(),
                j.at("time_increment").get<int>(),
                j.at("u_id_b").get<std::string>(),
                j.at("u_id_w").get<std::string>(),
                j.at("moves").get<std::vector<std::string>>(),
                j.at("esito").get<Esito>(),
                j.at("motivo").get<Motivo>()
        };
    }

    // Getters and Setters for time_duration
    int getTimeDuration() const { return time_duration; }
    void setTimeDuration(int duration) { time_duration = duration; }

    // Getters and Setters for time_increment
    int getTimeIncrement() const { return time_increment; }
    void setTimeIncrement(int increment) { time_increment = increment; }

    // Getters and Setters for u_id_b
    std::string getBlackPlayer() const { return u_id_b; }
    void setBlackPlayer(const std::string& black) { u_id_b = black; }

    // Getters and Setters for u_id_w
    std::string getWhitePlayer() const { return u_id_w; }
    void setWhitePlayer(const std::string& white) { u_id_w = white; }

    // Getters and Setters for moves
    std::vector<std::string> getMoves() const { return moves; }
    void setMoves(const std::vector<std::string>& game_moves) { moves = game_moves; }

    // Getters and Setters for esito
    Esito getEsito() const { return esito; }
    void setEsito(Esito game_esito) { esito = game_esito; }

    // Getters and Setters for motivo
    Motivo getMotivo() const { return motivo; }
    void setMotivo(Motivo game_motivo) { motivo = game_motivo; }

    // Method to display game details
    void display() const {
        std::cout << "Time Duration: " << time_duration << std::endl;
        std::cout << "Time Increment: " << time_increment << std::endl;
        std::cout << "Black Player: " << u_id_b << std::endl;
        std::cout << "White Player: " << u_id_w << std::endl;
        std::cout << "Moves: ";
        for (const auto& move : moves) {
            std::cout << move << " ";
        }
        std::cout << std::endl;
        std::cout << "Esito: " << static_cast<int>(esito) << std::endl;  // Conversione a int per la stampa
        std::cout << "Motivo: " << static_cast<int>(motivo) << std::endl;  // Conversione a int per la stampa
    }

    // Method to update the game
    void updateGame(int duration, int increment, const std::string& black, const std::string& white, const std::vector<std::string>& game_moves, Esito game_esito, Motivo game_motivo) {
        setTimeDuration(duration);
        setTimeIncrement(increment);
        setBlackPlayer(black);
        setWhitePlayer(white);
        setMoves(game_moves);
        setEsito(game_esito);
        setMotivo(game_motivo);
    }
};
class UserPreference {
private:
    Chessboard_style c_st;
    Pieces_style p_st;
    std::string u_id;

public:
    // Constructor
    UserPreference(const std::string& username, Chessboard_style chessboardStyle, Pieces_style piecesStyle)
            : u_id(username), c_st(chessboardStyle), p_st(piecesStyle) {}

    // Metodo per convertire l'oggetto UserPreference in JSON
    json toJson() const {
        return json{
                {"u_id", u_id},
                {"c_st", c_st},
                {"p_st", p_st}
        };
    }
    // Metodo statico per creare un oggetto UserPreference da JSON
    static UserPreference fromJson(const json& j) {
        return UserPreference{
                j.at("u_id").get<std::string>(),
                j.at("c_st").get<Chessboard_style>(),
                j.at("p_st").get<Pieces_style>()
        };
    }

    // Getter for chessboard_style
    Chessboard_style getChessboardStyle() const {
        return c_st;
    }

    // Setter for chessboard_style
    void setChessboardStyle(Chessboard_style style) {
        c_st = style;
    }

    // Getter for pieces_style
    Pieces_style getPiecesStyle() const {
        return p_st;
    }

    // Setter for pieces_style
    void setPiecesStyle(Pieces_style style) {
        p_st = style;
    }

    // Getter for username
    const std::string& getUsername() const {
        return u_id;
    }
};

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
    Server();

private:
    void handle_client(std::string);
    void listen_for_clients();

    int new_game(int duration,int increment,std::string u_id_b,std::string u_id_w);
    void search_opponent(std::string u_id, int duration,int increment);
    int update_game(int g_id, std::vector<std::string> moves, Esito e, Motivo m);
    Game search_game(int g_id);
    std::tuple<User,UserPreference> new_user(std::string username, std::string nome, std::string cognome, int elo, Chessboard_style c_st, Pieces_style p_st);
    User update_user(std::string username, std::string nome, std::string cognome, int elo);
    UserPreference update_userPreference(std::string username, Chessboard_style c_st, Pieces_style p_st);
    void delete_user(std::string username);

    std::vector<std::string> connected_clients;
    std::unordered_map<std::string, std::vector<std::string>> client_map;
    bool stop_server;
    RedisManager* redisManager;
    const std::string NEW_CLIENTS_CHANNEL = "newclients";
};

#endif //SERVER_SERVER_HPP
