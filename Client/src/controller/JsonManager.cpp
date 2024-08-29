#include "JsonManager.hpp"

void from_json(const json &j, Richiesta &r)
{
    r.codice = j.at("codice").get<int>();
    r.input = j.at("input").get<json>();
}

void to_json(json &j, const Richiesta &r)
{
    j = json{{"codice", r.codice}, {"input", r.input}};
}

void from_json(const json &j, Ricerca &r)
{
    r.user = j.at("user").get<std::string>();
    r.time_duration = j.at("time_duration").get<int>();
    r.time_increment = j.at("time_increment").get<int>();
}

void to_json(json &j, const Ricerca &r)
{
    j = json{{"user", r.user}, {"time_duration", r.time_duration}, {"time_increment", r.time_increment}};
}