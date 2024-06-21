#include "Database.hpp"

Database::Database() {
    conn = nullptr;
    res = nullptr;

    if (!Connect(default_connection)) return;

    if (!databaseExists(targetDbName)) {
        createDatabase(targetDbName);
    } else {
        std::cout << "Database " << targetDbName << " already exists." << std::endl;
    }

    Disconnect();

    if (!Connect(database_connection)) return;
}

Database::~Database() {
    Disconnect();
}

bool Database::Connect(const char* connection_string) {
    conn = PQconnectdb(default_connection);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database " << connection_string << " failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return false;
    }
    return true;
}

void Database::Disconnect() {
    PQfinish(conn);
}

bool Database::databaseExists(const char* dbName) {
    const char* query = "SELECT 1 FROM pg_database WHERE datname = $1";
    const char* paramValues[1] = {dbName};
    res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }
    bool exists = PQntuples(res) > 0;
    PQclear(res);
    return exists;
}

void Database::createDatabase(const char* dbName) {
    const char* query = "CREATE DATABASE $1";
    const char* paramValues[1] = {dbName};
    res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
    }
    PQclear(res);

    // Crea tabelle 
}

void Database::deleteDatabase(const char* dbName) {
    const char* query = "DROP DATABASE $1";
    const char* paramValues[1] = {dbName};
    res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
    }
    PQclear(res);
}

bool Database::ExecuteQuery(const char* query) {
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }
    return true;
}

PGresult* Database::GetResult() {
    return res;
}

void Database::createSchema() {
    const char* queries[] = {
        "CREATE TYPE Esito AS ENUM('B','N','P')",
        "CREATE TYPE Motivo AS ENUM('')",
        "CREATE TYPE Chessboard_style AS ENUM('blue','brown','black')",
        "CREATE TYPE Pieces_style AS ENUM('neo','pixel')",
        "CREATE TABLE User("
            "Username VARCHAR(20) PRIMARY KEY NOT NULL,"
            "Nome VARCHAR(20),"
            "Cognome VARCHAR(25),"
            "PuntiElo INTEGER NOT NULL"
        ")"
    };

    for (const char* query : queries) {
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Failed to create schema: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return;
        }
        PQclear(res);
    }
}

