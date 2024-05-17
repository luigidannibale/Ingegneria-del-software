#include "GameManager.h"

GameManager::GameManager(GameOptions* options){
    board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    if (options->GetStartSide() == StartSide::Casual) {
        int randomInt = std::rand() % 2;
        playerIsWhite = static_cast<bool>(randomInt);
    }
    else
        playerIsWhite = options->GetStartSide() == StartSide::White;
    turn = White;

    stockfishManager = new StockfishManager();
}
GameManager::~GameManager() {
    stockfishManager->close_stockfish();
}


#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <functional>


chess::Move GameManager::prova() {
    std::string fen = board.getFen();
    std::string bestmove = stockfishManager->get_bestmove(fen);
    stockfishManager->get_eval(fen);
    std::cout<< " Move to play now is " <<bestmove <<std::endl;

    std::string_view f = bestmove.substr(0,2);
    std::string_view t = bestmove.substr(2,2);

    chess::Square from = chess::Square(f);
    chess::Square to = chess::Square(t);
    chess::PieceType pt = board.at(from).type();

    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board, TypeToGenType(pt));
    for (const auto &move : moves) {
        if(move.from().operator==(from) && move.to().operator==(to)) {
            return move;
        }
    }
    return chess::Move();
}

void GameManager::makeComputerMove() {
    std::cout << "starting prova" << std::endl;
    std::future<chess::Move> result_future = std::async(std::launch::async, &GameManager::prova, this);
    // std::cout<< "--- I found the move: "<< chess::uci::moveToUci(result_future.get()) <<std::endl;
    std::cout << "after prova" << std::endl;

    board.makeMove(result_future.get());
    swapTurn();
}

bool GameManager::isWhite() {return playerIsWhite;}
chess::Board GameManager::GetBoard() {return board;}
void GameManager::updateBoard(chess::Board board) {this->board = board;}
void GameManager::swapTurn(){
    if (turn==White)    turn = Black;
    else                turn = White;
}
Turn GameManager::getTurn() {return turn;}
bool GameManager::playerCanPlay() {
    if ((playerIsWhite && turn == Turn::White) || (!playerIsWhite && turn == Turn::Black)) return true;
    return false;
}
chess::PieceGenType TypeToGenType(chess::PieceType piece){
    if(piece == chess::PieceType::NONE){
        std::cout << "Errore in TypeToGenType: mi hai passato NONE" << std::endl;
        return chess::PieceGenType::PAWN;
    }
    return static_cast<chess::PieceGenType>(std::pow(2, static_cast<int>(piece)));
}