#ifndef JSONMANAGER_HPP
#define JSONMANAGER_HPP

#include <nlohmann/json.hpp>
#include <string>
#include "../view/GameGraphicOptions.hpp"

using json = nlohmann::json;

enum class CodiceRichiesta
{
    new_game = 101,
    search_opponent = 102,
    update_game = 103,
    search_game = 104,
    quit_search_opponent = 105,
    new_user = 201,
    update_user = 202,
    update_userPreference = 203,
    delete_user = 204
};

enum class CodiceRisposta
{
    ok = 200,
    created = 201,
    game_created = 202,
    game_found = 203,
    user_created = 204,
    bad_request = 400,
    forbidden = 403,
    not_found = 404,
    server_error = 500,
};

struct Messaggio
{
    int codice;
    json input;
};

struct Ricerca
{
    std::string user;
    int time_duration;
    int time_increment;
};

void from_json(const json &j, Messaggio &r);
void to_json(json &j, const Messaggio &r);

void from_json(const json &j, Ricerca &r);
void to_json(json &j, const Ricerca &r);

void from_json(const json &j, BoardStyle &r);
void to_json(json &j, const BoardStyle &r);

void from_json(const json &j, PiecesStyle &r);
void to_json(json &j, const PiecesStyle &r);

#endif // JSONMANAGER_HPP