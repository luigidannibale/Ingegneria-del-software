#include "Server.hpp"

std::unordered_map<std::string, std::vector<std::string>> users;

Server::Server()
{
    this->stop_server = false;
    this->redisManager = new RedisManager();

    db = new Database();

    // Redis connection part
    redisManager->Connect();
    bool accepted = redisManager->SubscribeToChannel(NEW_CLIENTS_CHANNEL.c_str());
    std::string s = accepted ? "Connection accepted" : "Connection not accepted";
    std::cout << s << std::endl;
    listen_for_clients();

    // db->Disconnect();
    // db->ConnectDefault();
    // db->deleteDatabase("chess");
}

void Server::handle_client(std::string messaggio)
{
    json jmess;
    try
    {
        jmess = json::parse(messaggio);
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "Cannot parse message: " << messaggio << std::endl;
        return;
    }

    Richiesta r = Richiesta::fromJson(jmess);
    std::cout << "Received request: " << r.toJson().dump() << std::endl;

    switch (r.getCodice())
    {
    case CodiceRichiesta::new_game:
        new_game(r.getPayload().at("time_duration"),
                 r.getPayload().at("time_increment"),
                 r.getPayload().at("black"),
                 r.getPayload().at("white"));
        break;
    case CodiceRichiesta::search_opponent:
        search_opponent(r.getPayload().at("user"),
                        r.getPayload().at("time_duration"),
                        r.getPayload().at("time_increment"));
        break;
    case CodiceRichiesta::quit_search_opponent:
        search_opponent(r.getPayload().at("user"),
                        r.getPayload().at("time_duration"),
                        r.getPayload().at("time_increment"),
                        true);
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
    case CodiceRichiesta::list_games:
        list_games(r.getPayload().at("username"));
        break;
    case CodiceRichiesta::new_user:
        // new_user(r.getPayload().at("username"),
        //          r.getPayload().at("nome"),
        //          r.getPayload().at("cognome"),
        //          r.getPayload().at("elo"),
        //          r.getPayload().at("chessboard_style"),
        //          r.getPayload().at("pieces_style"));
        new_user(r.getPayload().at("username"));
        break;
    case CodiceRichiesta::update_user:
        update_user(r.getPayload().at("username"),
                    r.getPayload().at("new_username"),
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

void Server::listen_for_clients()
{
    while (!stop_server)
    {
        std::string message = redisManager->WaitResponse();
        if (message.empty())
            continue;
        std::thread(&Server::handle_client, this, message).detach();
    }
}

json Server::new_game(int duration, int increment, std::string u_id_b, std::string u_id_w)
{
    // int gameID = db->InsertNewGame(u_id_b.c_str(), u_id_w.c_str(), duration, increment);
    // if (gameID == -1)
    // {
    //     // errore
    //     return json();
    // }
    // // non errore
    return json();
}

json Server::search_opponent(std::string u_id, int duration, int increment, bool quit)
{
    std::string key = std::to_string(duration) + "+" + std::to_string(increment);

    if (quit) // User has quit the search
    {
        std::vector<std::string>::iterator it = std::find(users[key].begin(), users[key].end(), u_id);
        if (it != users[key].end())
            users[key].erase(it);

        json mess;
        mess["codice"] = CodiceRisposta::ok;
        mess["input"] = {};
        redisManager->PublishToChannel(u_id.c_str(), mess.dump().c_str());
        return json();
    }

    // Add user to the list of users searching for an opponent
    users[key].push_back(u_id);
    json mess;
    mess["codice"] = CodiceRisposta::ok;
    mess["input"] = {};
    redisManager->PublishToChannel(u_id.c_str(), mess.dump().c_str());

    if (users[key].size() >= 2)
    {
        std::string black = users[key].back();
        users[key].pop_back();
        std::string white = users[key].back();
        users[key].pop_back();
        try
        {
            json reply_white;
            reply_white["codice"] = CodiceRisposta::game_created;
            reply_white["input"]["opponent"] = black;
            reply_white["input"]["side"] = 0;

            json reply_black;
            reply_black["codice"] = CodiceRisposta::game_created;
            reply_black["input"]["opponent"] = white;
            reply_black["input"]["side"] = 1;

            std::cout << "Found opponent: " << white << " with side 0" << std::endl;
            std::cout << "Found opponent: " << black << " with side 1" << std::endl;

            // Create new game
            int gameID = db->InsertNewGame(white.c_str(), black.c_str(), duration, increment);
            if (gameID == -1)
            {
                throw std::runtime_error("Error creating game");
                return json();
            }

            reply_white["input"]["game_id"] = gameID;
            reply_black["input"]["game_id"] = gameID;

            redisManager->PublishToChannel(white.c_str(), reply_white.dump().c_str());
            redisManager->PublishToChannel(black.c_str(), reply_black.dump().c_str());
        }
        catch (std::exception &e)
        {
            mess["codice"] = CodiceRisposta::server_error;
            mess["input"] = {};
            redisManager->PublishToChannel(white.c_str(), mess.dump().c_str());
            redisManager->PublishToChannel(black.c_str(), mess.dump().c_str());
        }
    }
    return json();
}

json Server::update_game(int g_id, std::string moves, Esito e, Motivo m)
{
    json esito = e;
    json motivo = m;

    std::string esito_str = esito.get<std::string>();
    std::string motivo_str = motivo.get<std::string>();

    json mess;

    if (db->UpdateGame(g_id, moves.c_str(), esito_str.c_str(), motivo_str.c_str()))
    {
        // Game updated successfully
        mess["codice"] = CodiceRisposta::ok;
        mess["input"] = {};
    }
    else
    {
        // Error updating game
        mess["codice"] = CodiceRisposta::server_error;
        mess["input"] = {};
    }

    std::string channel = "game" + std::to_string(g_id);
    redisManager->PublishToChannel(channel.c_str(), mess.dump().c_str());
    return json();
}

json Server::search_game(int g_id)
{
    Game game = db->SearchGame(g_id);
    json mess;

    if (game.getID() == -1)
    {
        mess["codice"] = CodiceRisposta::not_found;
        mess["input"] = "{}";
    }
    else if (game.getID() == -2)
    {
        mess["codice"] = CodiceRisposta::server_error;
        mess["input"] = "{}";
    }
    else
    {
        mess["codice"] = CodiceRisposta::game_found;
        mess["input"] = game.toJson();
    }

    std::string channel = "game" + std::to_string(g_id);
    redisManager->PublishToChannel(channel.c_str(), mess.dump().c_str());
    return json();
}

json Server::list_games(std::string user_id)
{
    std::vector<Game> games;
    bool res = db->ListGames(user_id.c_str(), games);
    json mess;
    if (res)
    {
        mess["codice"] = CodiceRisposta::game_found;
        mess["input"] = json::array();
        for (Game game : games)
        {
            mess["input"].push_back(game.toJson());
        }
    }
    else
    {
        mess["codice"] = CodiceRisposta::server_error;
        mess["input"] = "Internal server error";
    }
    redisManager->PublishToChannel(user_id.c_str(), mess.dump().c_str());
    return json();
}

// json Server::new_user(std::string username, std::string nome, std::string cognome, int elo, Chessboard_style c_st,
//                       Pieces_style p_st)
// {
//     json chessboard = c_st;
//     json pieces = p_st;
//     db->InsertUser(username.c_str(), nome.c_str(), cognome.c_str(), elo, chessboard.dump().c_str(), pieces.dump().c_str());
//     return json();
// }

json Server::new_user(std::string username)
{
    User user;
    int res = db->FindUser(username.c_str(), user);
    json mess;

    if (res == 0) // User does not exist
    {
        json default_c_st = Chessboard_style::brown;
        json default_p_st = Pieces_style::neo;

        std::string default_c_st_str = default_c_st.get<std::string>();
        std::string default_p_st_str = default_p_st.get<std::string>();

        if (db->InsertUser(username.c_str(), "", "", 500, default_c_st_str, default_p_st_str))
        {
            user.setUsername(username);
            user.setNome("");
            user.setCognome("");
            user.setPuntiElo(500);
            user.setChessboardStyle(Chessboard_style::brown);
            user.setPiecesStyle(Pieces_style::neo);

            mess["codice"] = CodiceRisposta::user_created;
            mess["input"] = user.toJson();
        }
        else
        {
            mess["codice"] = CodiceRisposta::server_error;
            mess["input"] = "Internal server error";
        }
    }
    else if (res == 1) // User already exists
    {
        mess["codice"] = CodiceRisposta::user_created;
        mess["input"] = user.toJson();
    }
    else // Internal server error
    {
        mess["codice"] = CodiceRisposta::server_error;
        mess["input"] = "Internal server error";
    }

    redisManager->PublishToChannel(username.c_str(), mess.dump().c_str());
    return mess;
}

json Server::update_user(std::string username, std::string new_username, std::string nome, std::string cognome, int elo)
{
    json mess;
    mess["input"] = "{}"; // Default value
    if (db->UpdateUser(username.c_str(), new_username.c_str(), nome.c_str(), cognome.c_str(), elo))
    {
        mess["codice"] = CodiceRisposta::ok;
    }
    else
    {
        User dummy;
        int res = db->FindUser(new_username.c_str(), dummy);
        if (res == 1) // Username already taken
        {
            mess["codice"] = CodiceRisposta::bad_request;
        }
        else
        {
            mess["codice"] = CodiceRisposta::server_error;
        }
    }

    redisManager->PublishToChannel(username.c_str(), mess.dump().c_str());
    return mess;
}

json Server::update_userPreference(std::string username, Chessboard_style c_st, Pieces_style p_st)
{
    json mess;
    mess["input"] = "{}"; // Default value

    json chessboard = c_st;
    json pieces = p_st;

    std::string chessboard_str = chessboard.get<std::string>();
    std::string pieces_str = pieces.get<std::string>();

    if (db->UpdateUserPreference(username.c_str(), chessboard_str, pieces_str))
    {
        mess["codice"] = CodiceRisposta::ok;
    }
    else
    {
        mess["codice"] = CodiceRisposta::server_error;
    }

    redisManager->PublishToChannel(username.c_str(), mess.dump().c_str());
    return mess;
}

json Server::delete_user(std::string username)
{
    db->DeleteUser(username.c_str());
    return json();
}

int main()
{
    Server server = Server();
    return 0;
}

// Serializzazione e deserializzazione per Esito
// NLOHMANN_JSON_SERIALIZE_ENUM(Esito, {
//     {Esito::W, "W"},
//     {Esito::D, "D"},
//     {Esito::B, "B"},
//     {Esito::NF, "NF"}
// })

// Function to convert Esito to JSON
void to_json(json &j, const Esito &esito)
{
    switch (esito)
    {
    case Esito::W:
        j = "W";
        break;
    case Esito::D:
        j = "D";
        break;
    case Esito::B:
        j = "B";
        break;
    case Esito::NF:
        j = "NF";
        break;
    }
}

// Function to convert JSON to Esito
void from_json(const json &j, Esito &esito)
{
    std::string esitoStr = j.get<std::string>();
    if (esitoStr == "W")
        esito = Esito::W;
    else if (esitoStr == "D")
        esito = Esito::D;
    else if (esitoStr == "B")
        esito = Esito::B;
    else if (esitoStr == "NF")
        esito = Esito::NF;
}

// Serializzazione e deserializzazione per Motivo
// NLOHMANN_JSON_SERIALIZE_ENUM(Motivo, {
// {Motivo::checkmate, "checkmate"},
// {Motivo::wonOnTime, "wonOnTime"},
// {Motivo::quitmate, "quitmate"},
// {Motivo::stalemate, "stalemate"},
// {Motivo::insufficientMaterial, "insufficientMaterial"},
// {Motivo::threefoldRepetition, "threefoldRepetition"},
// {Motivo::fiftyMoveRule, "fiftyMoveRule"},
// {Motivo::NF, "NF"}
// })

void to_json(json &j, const Motivo &motivo)
{
    switch (motivo)
    {
    case Motivo::checkmate:
        j = "checkmate";
        break;
    case Motivo::wonOnTime:
        j = "wonOnTime";
        break;
    case Motivo::quitmate:
        j = "quitmate";
        break;
    case Motivo::stalemate:
        j = "stalemate";
        break;
    case Motivo::insufficientMaterial:
        j = "insufficientMaterial";
        break;
    case Motivo::threefoldRepetition:
        j = "threefoldRepetition";
        break;
    case Motivo::fiftyMoveRule:
        j = "fiftyMoveRule";
        break;
    case Motivo::NF:
        j = "NF";
        break;
    }
}

// Function to convert JSON to Motivo
void from_json(const json &j, Motivo &motivo)
{
    std::string motivoStr = j.get<std::string>();
    if (motivoStr == "checkmate")
        motivo = Motivo::checkmate;
    else if (motivoStr == "wonOnTime")
        motivo = Motivo::wonOnTime;
    else if (motivoStr == "quitmate")
        motivo = Motivo::quitmate;
    else if (motivoStr == "stalemate")
        motivo = Motivo::stalemate;
    else if (motivoStr == "insufficientMaterial")
        motivo = Motivo::insufficientMaterial;
    else if (motivoStr == "threefoldRepetition")
        motivo = Motivo::threefoldRepetition;
    else if (motivoStr == "fiftyMoveRule")
        motivo = Motivo::fiftyMoveRule;
    else if (motivoStr == "NF")
        motivo = Motivo::NF;
}
// Serializzazione e deserializzazione per Chessboard_Style
// NLOHMANN_JSON_SERIALIZE_ENUM(Chessboard_style, {
// {Chessboard_style::blue, "blue"},
// {Chessboard_style::brown, "brown"},
// {Chessboard_style::black, "black"}
// })
// // Serializzazione e deserializzazione per Pieces_style
// NLOHMANN_JSON_SERIALIZE_ENUM(Pieces_style, {
// {Pieces_style::neo, "neo"},
// {Pieces_style::pixel, "pixel"}
// })

void to_json(json &j, const Chessboard_style &chessboard_style)
{
    switch (chessboard_style)
    {
    case Chessboard_style::blue:
        j = "blue";
        break;
    case Chessboard_style::brown:
        j = "brown";
        break;
    case Chessboard_style::black:
        j = "black";
        break;
    }
}

void from_json(const json &j, Chessboard_style &chessboard_style)
{
    std::string chessboardStyleStr = j.get<std::string>();
    if (chessboardStyleStr == "blue")
        chessboard_style = Chessboard_style::blue;
    else if (chessboardStyleStr == "brown")
        chessboard_style = Chessboard_style::brown;
    else if (chessboardStyleStr == "black")
        chessboard_style = Chessboard_style::black;
}

void to_json(json &j, const Pieces_style &pieces_style)
{
    switch (pieces_style)
    {
    case Pieces_style::neo:
        j = "neo";
        break;
    case Pieces_style::pixel:
        j = "pixel";
        break;
    }
}

void from_json(const json &j, Pieces_style &pieces_style)
{
    std::string piecesStyleStr = j.get<std::string>();
    if (piecesStyleStr == "neo")
    {
        pieces_style = Pieces_style::neo;
    }
    else if (piecesStyleStr == "pixel")
    {
        pieces_style = Pieces_style::pixel;
    }
}
