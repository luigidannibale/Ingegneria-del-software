#include "Settings.h"

Settings::Settings(enum ChessboardStyle chessboardStyle,enum PiecesStyle piecesStyle) {
    this->chessboardStyle = chessboardStyle;
    this->piecesStyle = piecesStyle;
}
PiecesStyle Settings::GetPiecesStyle() {return piecesStyle;}
ChessboardStyle Settings::GetChessboardStyle() {return chessboardStyle;}
void Settings::SetChessboardStyle(enum ChessboardStyle chessboardStyle) {
    this->chessboardStyle = chessboardStyle;
}
void Settings::SetPiecesStyle(enum PiecesStyle piecesStyle) {
    this->piecesStyle = piecesStyle;
}