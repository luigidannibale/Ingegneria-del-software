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

    switch(options->GetComputerElo()) {
        case ComputerElo::Beginner:
            depthLevel = 2;
            break;
        case ComputerElo::Intermediate:
            depthLevel = 5;
            break;
        case ComputerElo::Advanced:
            depthLevel = 8;
            break;
        case ComputerElo::Master:
            depthLevel = 10;
            break;
        default:    // Should never happen
            depthLevel = 1;
            break;
    }
    // stockfishManager = new StockfishManager();
}
GameManager::~GameManager() {
    stockfishManager->close_stockfish();
}

void GameManager::StartStockfish() {
    stockfishManager = new StockfishManager();
}

chess::Move GameManager::GetBestMove() {
    std::string fen = board.getFen();

    int random = std::rand() % 3;
    random += depthLevel;

    std::string bestmove = stockfishManager->get_bestmove(fen, random);
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
    std::cout << "Errore in GetBestMove: mossa non trovata" << std::endl;
    return chess::Move(chess::Move::CASTLING).make(from, to, pt);
}

void GameManager::makeComputerMove() {
    std::cout << "starting prova" << std::endl;
    // std::future<chess::Move> result_future = std::async(std::launch::async, &GameManager::GetBestMove, this);
    // std::cout<< "--- I found the move: "<< chess::uci::moveToUci(result_future.get()) <<std::endl;
    std::cout << "after prova" << std::endl;

    // board.makeMove(result_future.get());
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