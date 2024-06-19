#ifndef INGEGNERIA_DEL_SOFTWARE_DATABASE_H
#define INGEGNERIA_DEL_SOFTWARE_DATABASE_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <postgresql/libpq-fe.h>

class Database {
public:

#include <iostream>
#include <libpq-fe.h> // PostgreSQL C library

    class Database {
    public:
        Database(const std::string& dbname, const std::string& user, const std::string& password, const std::string& host, const std::string& port)
                : dbname(dbname), user(user), password(password), host(host), port(port), conn(nullptr) {}

        ~Database() {
            disconnect();
        }

        bool connect() {
            std::string conninfo = "dbname=" + dbname + " user=" + user + " password=" + password + " host=" + host + " port=" + port;
            conn = PQconnectdb(conninfo.c_str());

            if (PQstatus(conn) != CONNECTION_OK) {
                std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
                disconnect();
                return false;
            }

            std::cout << "Connected to database successfully." << std::endl;
            return true;
        }

        void disconnect() {
            if (conn) {
                PQfinish(conn);
                conn = nullptr;
                std::cout << "Disconnected from database." << std::endl;
            }
        }

        void createSchema() {
            if (!conn) {
                std::cerr << "Not connected to database." << std::endl;
                return;
            }

            executeCommand("CREATE TYPE Esito AS ENUM ('Black_Win', 'White_Win', 'Draw')");
            executeCommand("CREATE TABLE Giocatore ("
                           "Username VARCHAR(30) PRIMARY KEY NOT NULL,"
                           "Nome VARCHAR(60),"
                           "Cognome VARCHAR(60),"
                           "Password VARCHAR(15) NOT NULL,"
                           "PuntiElo INTEGER"
                           ")");
            executeCommand("CREATE TABLE Preferenza ("
                           "Giocatore VARCHAR(30) NOT NULL REFERENCES Giocatore(Username),"
                           "Tema INTEGER NOT NULL REFERENCES Tema(ID)"
                           ")");
            executeCommand("CREATE TABLE Tema ("
                           "ID SERIAL PRIMARY KEY,"
                           "ColoreScacchiera INTEGER,"
                           "TemaPedine INTEGER"
                           ")");
            executeCommand("CREATE TABLE Partita ("
                           "ID SERIAL PRIMARY KEY,"
                           "GiocatoreBianco VARCHAR(30) NOT NULL REFERENCES Giocatore(Username),"
                           "GiocatoreNero VARCHAR(30) NOT NULL REFERENCES Giocatore(Username),"
                           "Cadenza VARCHAR(20),"
                           "Fen VARCHAR(100),"
                           "Esito Esito"
                           ")");
            executeCommand("CREATE TABLE Logs ("
                           "ID SERIAL PRIMARY KEY,"
                           "Azione VARCHAR(200) NOT NULL,"
                           "Timestamp TIMESTAMP NOT NULL"
                           ")");
        }

    private:
        std::string dbname;
        std::string user;
        std::string password;
        std::string host;
        std::string port;
        PGconn* conn;

        void executeCommand(const std::string& sql) {
            PGresult *res = PQexec(conn, sql.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Error executing command: " << PQerrorMessage(conn) << std::endl;
                PQclear(res);
                throw std::runtime_error("Failed to execute SQL command.");
            }
            PQclear(res);
        }
    };

    int main() {
        // Replace with your actual database connection details
        Database db("mydatabase", "myuser", "mypassword", "localhost", "5432");

        if (db.connect()) {
            db.createSchema();
            db.disconnect();
        }

        return 0;
    }


private:
    pqxx::connection conn;
    const char* connection_string = "dbname=chessdb user=luigi password=luigi host=localhost hostaddr=127.0.0.1 port=5432";
};




#endif //INGEGNERIA_DEL_SOFTWARE_DATABASE_H
