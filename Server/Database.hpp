#include <libpq-fe.h>
#include <iostream>

class Database {
public:
    Database();
    ~Database();
    bool Connect();
    void Disconnect();
    bool ExecuteQuery(const char* query);
    PGresult* GetResult();
    void createSchema();

private:
    const char* default_connection = "user=postgres password=postgres hostnames=localhost port=5432 dbname=postgres";
    const char* targetDbName = "chess";
    const char* database_connection = "user=postgres password=postgres hostnames=localhost port=5432 dbname=chess";

    bool databaseExists(const char* dbName);

    PGconn* conn;
}