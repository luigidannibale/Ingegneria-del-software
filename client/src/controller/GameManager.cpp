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
}
GameManager::~GameManager() {}
bool GameManager::isWhite() {return playerIsWhite;}
chess::Board GameManager::GetBoard() {return board;}
void GameManager::updateBoard(chess::Board board) {this->board = board;}
void GameManager::swapTurn(){
    if (turn==White) turn = Black;
    else    turn = White;

}
Turn GameManager::getTurn() {return turn;}
bool GameManager::playerCanPlay() {
    if ((playerIsWhite && turn == Turn::White) || (!playerIsWhite && turn == Turn::Black)){
        return true;
    }
    return false;
}