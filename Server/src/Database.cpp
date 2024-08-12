#include "Database.hpp"

Database::Database()
{
    conn = nullptr;
    res = nullptr;
    std::cerr << "1" << std::endl;
    if (!Connect(default_connection))
        return;
    std::cerr << "2" << std::endl;
    if (!databaseExists(targetDbName))
    {
        createDatabase(targetDbName);
    }
    else
    {
        std::cout << "Database " << targetDbName << " already exists." << std::endl;
    }
    std::cerr << "3" << std::endl;
    Disconnect();
    std::cerr << "4" << std::endl;
    if (!Connect(database_connection))
        return;
}

Database::~Database()
{
    deleteDatabase(targetDbName);
    Disconnect();
}

bool Database::ConnectDefault()
{
    return Connect(default_connection);
}

bool Database::Connect(const char *connection_string)
{
    conn = PQconnectdb(connection_string);
    if (PQstatus(conn) != CONNECTION_OK)
    {
        std::cerr << "Connection to database " << connection_string << " failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return false;
    }
    return true;
}

void Database::Disconnect()
{
    PQfinish(conn);
}

// Function to escape string literals
std::string escape_literal(PGconn *conn, const std::string &literal)
{
    char *escaped = PQescapeLiteral(conn, literal.c_str(), literal.length());
    if (!escaped)
    {
        std::cerr << "Escaping literal failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
    std::string escaped_literal(escaped);
    PQfreemem(escaped);
    return escaped_literal;
}

// Function to escape identifiers
std::string escape_identifier(PGconn *conn, const std::string &identifier)
{
    char *escaped = PQescapeIdentifier(conn, identifier.c_str(), identifier.length());
    if (!escaped)
    {
        std::cerr << "Escaping identifier failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
    std::string escaped_identifier(escaped);
    PQfreemem(escaped);
    return escaped_identifier;
}

bool Database::databaseExists(const char *dbName)
{
    std::string query = "SELECT 1 FROM pg_database WHERE datname = " + escape_literal(conn, dbName);
    ;
    res = PQexec(conn, query.c_str());
    // res = PQexecParams(conn, query.c_str(), 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }
    bool exists = PQntuples(res) > 0;
    PQclear(res);
    return exists;
}

void Database::createDatabase(const char *dbName)
{
    std::string query = "CREATE DATABASE " + escape_identifier(conn, dbName);
    res = PQexec(conn, query.c_str());
    // res = PQexecParams(conn, query.c_str(), 0, nullptr, nullptr, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
    }
    PQclear(res);

    // Crea tabelle
    createSchema();
}

void Database::deleteDatabase(const char *dbName)
{
    std::string query = "DROP DATABASE IF EXISTS " + escape_identifier(conn, dbName);
    res = PQexec(conn, query.c_str());
    // res = PQexecParasms(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return;
    }
    std::cout << "Database " << dbName << " deleted.\n";
    PQclear(res);
}

bool Database::ExecuteQuery(const char *query)
{
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }
    return true;
}

PGresult *Database::GetResult()
{
    return res;
}

void Database::InsertUser(const char *username, const char *nome, const char *cognome, int elo, const char *chessboard_style, const char *pieces_style)
{
    std::string query = "INSERT INTO Player (Username, Nome, Cognome, PuntiElo, Theme) VALUES (" + escape_literal(conn, username) + ", " + escape_literal(conn, nome) + ", " + escape_literal(conn, cognome) + ", " + std::to_string(elo) + ", " + "(SELECT ID FROM Theme WHERE ChessboardColor = " + escape_literal(conn, chessboard_style) + " AND PiecesColor = " + escape_literal(conn, pieces_style) + ")" + ");";
    if (ExecuteQuery(query.c_str()))
    {
        PQclear(res);
        // return username ??
    }
}

void Database::UpdateUser(const char *username, const char *nome, const char *cognome, int elo)
{
    std::string query = "UPDATE Player SET Nome = " + escape_literal(conn, nome) +
                        ", Cognome = " + escape_literal(conn, cognome) +
                        ", PuntiElo = " + std::to_string(elo) +
                        " WHERE Username = " + escape_literal(conn, username) + ";";
    if (ExecuteQuery(query.c_str()))
    {
        PQclear(res);
        // return username ??
    }
}

void Database::DeleteUser(const char *username)
{
    std::string query = "DELETE FROM Player WHERE Username = " + escape_literal(conn, username) + ";";
    if (ExecuteQuery(query.c_str()))
    {
        PQclear(res);
        // return true
    }
    // return false
}

int Database::InsertNewGame(const char *white, const char *black, int timeDuration, int timeIncrement)
{
    std::string query = "INSERT INTO Game (White, Black, TimeDuration, TimeIncrement, Moves, Esito, Motivo) VALUES (" + escape_literal(conn, white) + ", " + escape_literal(conn, black) + ", " + std::to_string(timeDuration) + ", " + std::to_string(timeIncrement) + ", " + "NULL, " + "NULL, " + "NULL) RETURNING ID;";
    if (ExecuteQuery(query.c_str()))
    {
        int gameId = std::stoi(PQgetvalue(res, 0, 0));
        PQclear(res);
        return gameId;
    }
    return -1;
}

void Database::UpdateGame(int gameId, const char *moves, const char *esito, const char *motivo)
{
    std::string query = "UPDATE Game SET Moves = " + escape_literal(conn, moves) +
                        ", Esito = " + escape_literal(conn, esito) +
                        ", Motivo = " + escape_literal(conn, motivo) +
                        " WHERE ID = " + std::to_string(gameId) + ";";
    if (ExecuteQuery(query.c_str()))
    {
        PQclear(res);
        // return gameId ??
    }
}

Game Database::SearchGame(int gameId)
{
    std::string query = "SELECT * FROM Game WHERE ID = " + std::to_string(gameId) + ";";
    if (ExecuteQuery(query.c_str()))
    {
        json esito = PQgetvalue(res, 0, 6);
        json motivo = PQgetvalue(res, 0, 7);

        Game game(std::stoi(PQgetvalue(res, 0, 0)), // ID
                  PQgetvalue(res, 0, 1),            // White
                  PQgetvalue(res, 0, 2),            // Black
                  std::stoi(PQgetvalue(res, 0, 3)), // TimeDuration
                  std::stoi(PQgetvalue(res, 0, 4)), // TimeIncrement
                  PQgetvalue(res, 0, 5),            // Moves
                  esito.get<Esito>(),               // Esito
                  motivo.get<Motivo>());            // Motivo
        PQclear(res);
        return game;
    }
    Game emptyGame;
    return emptyGame;
}

void Database::createSchema()
{
    const char *queries[] = {
        "CREATE TYPE Esito AS ENUM('W','D','B','NF');",
        "CREATE TYPE Motivo AS ENUM('checkmate','wonOnTime','quitmate','stalemate','insufficientMaterial','threefoldRepetition','fiftyMoveRule','NF');",
        "CREATE TYPE Chessboard_style AS ENUM('blue','brown','black');",
        "CREATE TYPE Pieces_style AS ENUM('neo','pixel');",

        "CREATE TABLE Theme ("
        "ID SERIAL PRIMARY KEY NOT NULL,"
        "ChessboardColor Chessboard_style NOT NULL,"
        "PiecesColor Pieces_style NOT NULL"
        ");",
        "CREATE TABLE Player ("
        "Username VARCHAR(20) PRIMARY KEY NOT NULL,"
        "Nome VARCHAR(20) NOT NULL,"
        "Cognome VARCHAR(25) NOT NULL,"
        "PuntiElo INTEGER NOT NULL,"
        // "Theme INTEGER NOT NULL REFERENCES Theme(ID)"
        "ChessboardColor Chessboard_style,"
        "PiecesColor Pieces_style"
        ");",
        // In teoria basta mettere il tema nella tabella user
        /*"CREATE TABLE UserPreference("
        "Username VARCHAR(20) PRIMARY KEY REFERENCES Player(Username),"
        "Theme INTEGER NOT NULL REFERENCES Theme(ID),"
        ")",*/

        "CREATE TABLE Game("
        "ID SERIAL PRIMARY KEY NOT NULL, "
        "White VARCHAR(20) NOT NULL REFERENCES Player(Username),"
        "Black VARCHAR(20) NOT NULL REFERENCES Player(Username),"
        "TimeDuration INTEGER NOT NULL,"
        "TimeIncrement INTEGER NOT NULL,"
        "Moves VARCHAR(500),"
        "Esito Esito,"
        "Motivo Motivo"
        ");",
        "CREATE TABLE Log("
        "ID SERIAL PRIMARY KEY NOT NULL, "
        "Azione VARCHAR(200) NOT NULL,"
        "Timestamp TIMESTAMP NOT NULL"
        ");",
    };

    for (const char *query : queries)
    {
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            if (strcmp(PQresultErrorField(res, PG_DIAG_SQLSTATE), "42710") == 0)
            {
                std::cerr << "Type already exists: " << PQerrorMessage(conn) << std::endl;
                continue;
            }
            if (strcmp(PQresultErrorField(res, PG_DIAG_SQLSTATE), "42P07") == 0)
            {
                std::cerr << "Table already exists: " << PQresultErrorMessage(res) << std::endl;
                continue;
            }
            std::cerr << "Failed to create schema: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return;
        }
        PQclear(res);
    }
}
