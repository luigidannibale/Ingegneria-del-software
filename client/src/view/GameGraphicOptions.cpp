//
// Created by luigi on 17/06/24.
//

#include "GameGraphicOptions.h"

GameGraphicOptions::GameGraphicOptions(BoardStyle bs, PiecesStyle ps){
    boardStyle = bs;
    piecesStyle = ps;
}

BoardStyle GameGraphicOptions::GetBoardStyle() {return boardStyle;}
PiecesStyle GameGraphicOptions::GetPiecesStyle() {return piecesStyle;}

void GameGraphicOptions::SetBoardStyle(BoardStyle bs) {boardStyle = bs;}
void GameGraphicOptions::SetPiecesStyle(PiecesStyle ps) {piecesStyle = ps;}