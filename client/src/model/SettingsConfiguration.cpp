#include "SettingsConfiguration.h"

SettingsConfiguration::SettingsConfiguration(Chessboard chessboard, Pieces pieces) {
    SetChessboard(chessboard);
    SetPieces(pieces);
}
Chessboard SettingsConfiguration::GetChessboard() {return chessboard;}
Pieces SettingsConfiguration::GetPieces() {return pieces;}

void SettingsConfiguration::SetChessboard(Chessboard chessboard)
{this->chessboard = chessboard;}
void SettingsConfiguration::SetPieces(Pieces pieces)
 {this->pieces = pieces;}