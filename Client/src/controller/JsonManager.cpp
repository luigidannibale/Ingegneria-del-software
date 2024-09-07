#include "JsonManager.hpp"

void from_json(const json &j, Messaggio &r)
{
    r.codice = j.at("codice").get<int>();
    r.input = j.at("input").get<json>();
}

void to_json(json &j, const Messaggio &r)
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

void to_json(json &j, const BoardStyle &chessboard_style)
{
    switch (chessboard_style)
    {
    case BoardStyle::blue:
        j = "blue";
        break;
    case BoardStyle::brown:
        j = "brown";
        break;
    case BoardStyle::black:
        j = "black";
        break;
    }
}

void from_json(const json &j, BoardStyle &chessboard_style)
{
    std::string chessboardStyleStr = j.get<std::string>();
    if (chessboardStyleStr == "blue")
        chessboard_style = BoardStyle::blue;
    else if (chessboardStyleStr == "brown")
        chessboard_style = BoardStyle::brown;
    else if (chessboardStyleStr == "black")
        chessboard_style = BoardStyle::black;
}

void to_json(json &j, const PiecesStyle &pieces_style)
{
    switch (pieces_style)
    {
    case PiecesStyle::neo:
        j = "neo";
        break;
    case PiecesStyle::pixel:
        j = "pixel";
        break;
    }
}

void from_json(const json &j, PiecesStyle &pieces_style)
{
    std::string piecesStyleStr = j.get<std::string>();
    if (piecesStyleStr == "neo")
    {
        pieces_style = PiecesStyle::neo;
    }
    else if (piecesStyleStr == "pixel")
    {
        pieces_style = PiecesStyle::pixel;
    }
}