#include "Server.hpp"

// Serializzazione e deserializzazione per Esito
// NLOHMANN_JSON_SERIALIZE_ENUM(Esito, {
//     {Esito::W, "W"},
//     {Esito::D, "D"},
//     {Esito::B, "B"},
//     {Esito::NF, "NF"}
// })

// Function to convert Esito to JSON
void esito_to_json(json &j, const Esito &esito)
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
void esito_from_json(const json &j, Esito &esito)
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

void motivo_to_json(json &j, const Motivo &motivo)
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
void motivo_from_json(const json &j, Motivo &motivo)
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

void chessboard_to_json(json &j, const Chessboard_style &chessboard_style)
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

void chessboard_from_json(const json &j, Chessboard_style &chessboard_style)
{
    std::string chessboardStyleStr = j.get<std::string>();
    if (chessboardStyleStr == "blue")
        chessboard_style = Chessboard_style::blue;
    else if (chessboardStyleStr == "brown")
        chessboard_style = Chessboard_style::brown;
    else if (chessboardStyleStr == "black")
        chessboard_style = Chessboard_style::black;
}

void pieces_to_json(json &j, const Pieces_style &pieces_style)
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

void pieces_from_json(const json &j, Pieces_style &pieces_style)
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
    json jmess = json::parse(messaggio);
    Richiesta r = Richiesta::fromJson(jmess);
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
    int gameID = db->InsertNewGame(u_id_b.c_str(), u_id_w.c_str(), duration, increment);
    if (gameID == -1)
    {
        // errore
        return json();
    }
    // non errore
    return json();
}

json Server::search_opponent(std::string u_id, int duration, int increment)
{
    return json();
}

json Server::update_game(int g_id, std::string moves, Esito e, Motivo m)
{
    json esito = e;
    json motivo = m;
    db->UpdateGame(g_id, moves.c_str(), esito.dump().c_str(), motivo.dump().c_str());
    return json();
}

json Server::search_game(int g_id)
{
    Game game = db->SearchGame(g_id);
    return json();
}

json Server::new_user(std::string username, std::string nome, std::string cognome, int elo, Chessboard_style c_st,
                      Pieces_style p_st)
{
    json chessboard = c_st;
    json pieces = p_st;
    db->InsertUser(username.c_str(), nome.c_str(), cognome.c_str(), elo, chessboard.dump().c_str(), pieces.dump().c_str());
    return json();
}

json Server::update_user(std::string username, std::string nome, std::string cognome, int elo)
{
    db->UpdateUser(username.c_str(), nome.c_str(), cognome.c_str(), elo);
    return json();
}

json Server::update_userPreference(std::string username, Chessboard_style c_st, Pieces_style p_st)
{
    return json();
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