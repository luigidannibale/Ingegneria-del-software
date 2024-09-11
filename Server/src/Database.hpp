#include <postgresql/libpq-fe.h>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include <tuple>

using json = nlohmann::json;

// Enumeration for esito
enum class Esito
{
    W, // Win
    D, // Draw
    B, // Banned
    NF // Not Finished
};
// Enumeration for Motivo
enum class Motivo
{
    checkmate,            // Checkmate
    wonOnTime,            // Won on Time
    quitmate,             // Quitmate
    stalemate,            // Stalemate
    insufficientMaterial, // Insufficient Material
    threefoldRepetition,  // Threefold Repetition
    fiftyMoveRule,        // 50-Move Rule
    NF                    // Not Finished
};

// Enum for chessboard_style
enum class Chessboard_style
{
    blue,
    brown,
    black
};
// Enum for pieces_style, using chessboard_style as underlying type
enum class Pieces_style
{
    neo,
    pixel
};

class Game
{
private:
    int ID;
    int time_duration;
    int time_increment;
    std::string u_id_b;
    std::string u_id_w;
    std::string moves;
    Esito esito;
    Motivo motivo;

public:
    Game() {};
    // Constructor
    Game(int id, std::string white, std::string black, int duration, int increment, std::string game_moves, Esito game_esito, Motivo game_motivo)
        : ID(id), time_duration(duration), time_increment(increment), u_id_b(black), u_id_w(white), moves(game_moves), esito(game_esito), motivo(game_motivo) {}

    // Game(int id, const char *black, const char *white, int duration, int increment, const char *game_moves, Esito game_esito, Motivo game_motivo)
    //     : ID(id), time_duration(duration), time_increment(increment), u_id_b(black), u_id_w(white), esito(game_esito), motivo(game_motivo)
    // {
    //     std::string moves_str(game_moves);
    //     std::string delimiter = " ";
    //     size_t pos = 0;
    //     std::string token;
    //     while ((pos = moves_str.find(delimiter)) != std::string::npos)
    //     {
    //         token = moves_str.substr(0, pos);
    //         moves.push_back(token);
    //         moves_str.erase(0, pos + delimiter.length());
    //     }
    //     moves.push_back(moves_str);
    // }

    // Metodo per convertire l'oggetto Game in JSON
    json toJson() const
    {
        return json{
            {"ID", ID},
            {"time_duration", time_duration},
            {"time_increment", time_increment},
            {"u_id_b", u_id_b},
            {"u_id_w", u_id_w},
            {"moves", moves},
            {"esito", esito},
            {"motivo", motivo}};
    }

    // Metodo statico per creare un oggetto Game da JSON
    static Game fromJson(const json &j)
    {
        return Game{
            j.at("ID").get<int>(),
            j.at("u_id_b").get<std::string>(),
            j.at("u_id_w").get<std::string>(),
            j.at("time_duration").get<int>(),
            j.at("time_increment").get<int>(),
            j.at("moves").get<std::string>(),
            j.at("esito").get<Esito>(),
            j.at("motivo").get<Motivo>()};
    }

    // Getter and Setter for ID
    int getID() const { return ID; }
    void setID(int id) { ID = id; }

    // Getters and Setters for time_duration
    int getTimeDuration() const { return time_duration; }
    void setTimeDuration(int duration) { time_duration = duration; }

    // Getters and Setters for time_increment
    int getTimeIncrement() const { return time_increment; }
    void setTimeIncrement(int increment) { time_increment = increment; }

    // Getters and Setters for u_id_b
    std::string getBlackPlayer() const { return u_id_b; }
    void setBlackPlayer(const std::string &black) { u_id_b = black; }

    // Getters and Setters for u_id_w
    std::string getWhitePlayer() const { return u_id_w; }
    void setWhitePlayer(const std::string &white) { u_id_w = white; }

    // Getters and Setters for moves
    std::string getMoves() const { return moves; }
    void setMoves(const std::string &game_moves) { moves = game_moves; }

    // Getters and Setters for esito
    Esito getEsito() const { return esito; }
    void setEsito(Esito game_esito) { esito = game_esito; }

    // Getters and Setters for motivo
    Motivo getMotivo() const { return motivo; }
    void setMotivo(Motivo game_motivo) { motivo = game_motivo; }

    // Method to display game details
    void display() const
    {
        std::cout << "Time Duration: " << time_duration << std::endl;
        std::cout << "Time Increment: " << time_increment << std::endl;
        std::cout << "Black Player: " << u_id_b << std::endl;
        std::cout << "White Player: " << u_id_w << std::endl;
        std::cout << "Moves: ";
        for (const auto &move : moves)
        {
            std::cout << move << " ";
        }
        std::cout << std::endl;
        std::cout << "Esito: " << static_cast<int>(esito) << std::endl;   // Conversione a int per la stampa
        std::cout << "Motivo: " << static_cast<int>(motivo) << std::endl; // Conversione a int per la stampa
    }

    // Method to update the game
    void updateGame(int duration, int increment, const std::string &black, const std::string &white, const std::string &game_moves, Esito game_esito, Motivo game_motivo)
    {
        setTimeDuration(duration);
        setTimeIncrement(increment);
        setBlackPlayer(black);
        setWhitePlayer(white);
        setMoves(game_moves);
        setEsito(game_esito);
        setMotivo(game_motivo);
    }
};

class User
{
public:
    User() {}
    // Constructor
    User(const std::string &username, const std::string &nome, const std::string &cognome, int puntiElo, Chessboard_style c_st, Pieces_style p_st)
        : username_(username), nome_(nome), cognome_(cognome), puntiElo_(puntiElo), c_st_(c_st), p_st_(p_st) {}

    // Metodo per convertire l'oggetto User in JSON
    json toJson() const
    {
        return json{
            {"username", username_},
            {"nome", nome_},
            {"cognome", cognome_},
            {"puntiElo", puntiElo_},
            {"c_st", c_st_},
            {"p_st", p_st_}};
    }

    // Metodo statico per creare un oggetto User da JSON
    static User fromJson(const json &j)
    {
        return User{
            j.at("username").get<std::string>(),
            j.at("nome").get<std::string>(),
            j.at("cognome").get<std::string>(),
            j.at("puntiElo").get<int>(),
            j.at("c_st").get<Chessboard_style>(),
            j.at("p_st").get<Pieces_style>()};
    }

    // Getter and Setter for Username
    std::string getUsername() const { return username_; }
    void setUsername(const std::string &username) { username_ = username; }

    // Getter and Setter for Nome
    std::string getNome() const { return nome_; }
    void setNome(const std::string &nome) { nome_ = nome; }

    // Getter and Setter for Cognome
    std::string getCognome() const { return cognome_; }
    void setCognome(const std::string &cognome) { cognome_ = cognome; }

    // Getter and Setter for PuntiElo
    int getPuntiElo() const { return puntiElo_; }
    void setPuntiElo(int puntiElo) { puntiElo_ = puntiElo; }

    // Getter and Setter for Chessboard_style
    Chessboard_style getChessboardStyle() const { return c_st_; }
    void setChessboardStyle(Chessboard_style style) { c_st_ = style; }

    // Getter and Setter for Pieces_style
    Pieces_style getPiecesStyle() const { return p_st_; }
    void setPiecesStyle(Pieces_style style) { p_st_ = style; }

    // Method to display player details
    void display() const
    {
        std::cout << "Username: " << username_ << std::endl;
        std::cout << "Nome: " << nome_ << std::endl;
        std::cout << "Cognome: " << cognome_ << std::endl;
        std::cout << "Punti Elo: " << puntiElo_ << std::endl;
        std::cout << "Chessboard Style: " << static_cast<int>(c_st_) << std::endl;
        std::cout << "Pieces Style: " << static_cast<int>(p_st_) << std::endl;
    }

private:
    std::string username_;
    std::string nome_;
    std::string cognome_;
    int puntiElo_;
    Chessboard_style c_st_;
    Pieces_style p_st_;
};

class Database
{

public:
    Database();
    ~Database();
    bool Connect(const char *connection_string);
    bool ConnectDefault();
    void Disconnect();
    bool IsOkay();
    bool ExecuteQuery(const char *query);
    PGresult *GetResult();
    bool createSchema();
    void deleteSchema();
    void createDatabase(const char *dbName);
    void deleteDatabase(const char *dbName);

    bool InsertUser(const char *username, const char *nome, const char *cognome, int elo, const std::string chessboard_style, const std::string pieces_style);
    bool UpdateUser(const char *username, const char *new_username, const char *nome, const char *cognome, int elo);
    int UpdateUserPreference(const char *username, const std::string chessboard_style, const std::string pieces_style);
    void DeleteUser(const char *username);
    int FindUser(const char *username, User &user);

    int InsertNewGame(const char *white, const char *black, int timeDuration, int timeIncrement);
    int UpdateGame(int game_id, const char *moves, const char *esito, const char *motivo);
    Game SearchGame(int game_id);
    bool ListGames(const char *username, std::vector<Game> &games);

private:
    const char *default_connection = "user=postgres password=postgres host=localhost port=5432 dbname=postgres";
    // const char *conninfo = "user=postgres password=postgres host=localhost port=5432 dbname=postgres";

    const char *targetDbName = "chess";
    const char *database_connection = "user=postgres password=postgres host=localhost port=5432 dbname=chess";

    bool databaseExists(const char *dbName);

    bool isOkay = false;
    PGconn *conn;
    PGresult *res;
};

class UserPreference
{
private:
    Chessboard_style c_st;
    Pieces_style p_st;
    std::string u_id;

public:
    // Constructor
    UserPreference(const std::string &username, Chessboard_style chessboardStyle, Pieces_style piecesStyle)
        : u_id(username), c_st(chessboardStyle), p_st(piecesStyle) {}

    // Metodo per convertire l'oggetto UserPreference in JSON
    json toJson() const
    {
        return json{
            {"u_id", u_id},
            {"c_st", c_st},
            {"p_st", p_st}};
    }
    // Metodo statico per creare un oggetto UserPreference da JSON
    static UserPreference fromJson(const json &j)
    {
        return UserPreference{
            j.at("u_id").get<std::string>(),
            j.at("c_st").get<Chessboard_style>(),
            j.at("p_st").get<Pieces_style>()};
    }

    // Getter for chessboard_style
    Chessboard_style getChessboardStyle() const
    {
        return c_st;
    }

    // Setter for chessboard_style
    void setChessboardStyle(Chessboard_style style)
    {
        c_st = style;
    }

    // Getter for pieces_style
    Pieces_style getPiecesStyle() const
    {
        return p_st;
    }

    // Setter for pieces_style
    void setPiecesStyle(Pieces_style style)
    {
        p_st = style;
    }

    // Getter for username
    const std::string &getUsername() const
    {
        return u_id;
    }
};