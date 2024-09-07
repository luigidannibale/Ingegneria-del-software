#ifndef GAMEPLAYCONTROLLER_H
#define GAMEPLAYCONTROLLER_H

#include "GameManager.hpp"
#include "RedisManager.hpp"
#include "../lib.hpp"
#include "../view/GameplayFrame.hpp"
#include "../model/GameOptions.hpp"
#include "../model/chess_pachage/chess.hpp"
#include "JsonManager.hpp"

#include <map>
#include <wx/utils.h>
#include <thread>
#include <random>
#include <string_view>

enum GameResult
{
    WIN,
    DRAW
};

enum class ResultReason
{
    CHECKMATE = static_cast<int>(chess::GameResultReason::CHECKMATE),
    STALEMATE = static_cast<int>(chess::GameResultReason::STALEMATE),
    INSUFFICIENT_MATERIAL = static_cast<int>(chess::GameResultReason::INSUFFICIENT_MATERIAL),
    FIFTY_MOVE_RULE = static_cast<int>(chess::GameResultReason::FIFTY_MOVE_RULE),
    THREEFOLD_REPETITION = static_cast<int>(chess::GameResultReason::THREEFOLD_REPETITION),
    TIME_OVER,
    QUITMATE
};

struct Game
{
    int ID;
    int time_duration;
    int time_increment;
    std::string u_id_b;
    std::string u_id_w;
    std::string moves;
    GameResult esito;
    ResultReason motivo;
};

void from_json(const json &j, Game &r);

void to_json(nlohmann::json &j, const GameResult &result);
void from_json(const nlohmann::json &j, GameResult &result);
void to_json(nlohmann::json &j, const ResultReason &reason);
void from_json(const nlohmann::json &j, ResultReason &reason);

struct CellCoordinates
{
public:
    CellCoordinates(int row, int col, std::string coordinates)
    {
        this->row = row;
        this->col = col;
        this->coordinates = coordinates;
    }
    int row;
    int col;
    std::string coordinates;
};

class GameplayController
{
public:
    GameplayController(wxPanel *, GameOptions *, GameGraphicOptions *, RedisManager * = nullptr, std::string user_id = "", std::string opponent_id = "", int gameId = -1);
    GameplayController(wxPanel *, GameGraphicOptions *, Game, bool);
    ~GameplayController();

private:
    // bool isWhite;
    bool gameClosed = false;
    bool gameEnded = false;
    bool isReplay = false;
    GameManager *gameManager;
    RedisManager *redisManager;
    wxPanel *returnPanel;
    std::string user_id;
    std::string opponent_id;
    std::string playedMoves;
    std::vector<chess::Move> movesHistory;
    int currentMove = 0;
    int gameId;
    CellCoordinates *clickedCoord = nullptr;
    GameplayFrame *frame;

    GameResult gameResult;
    ResultReason resultReason;

    const int MS_STOCKFISH_DELAY = 2000;

    // chess::Board board;
    chess::Movelist moves;
    std::map<chess::Square, chess::Move> playableMoves;
    void makeMove(std::string_view);
    void markFeasible(chess::Move);
    void printMove(chess::Piece, chess::Move, bool, bool);
    void unmarkFeasibles();
    bool CheckCheckmate();
    void AsyncComputerMove();
    void PublishHumanMove(std::string);
    void AsyncHumanMove();

    void UpdateChessboard();
    void OnClose(wxCloseEvent &);
    void ClickBoard(wxMouseEvent &);
    void UpdateTime(wxTimerEvent &);

    void OnNextMove(wxCommandEvent &);
    void OnPrevMove(wxCommandEvent &);
};

#endif // GAMEPLAYCONTROLLER_H
