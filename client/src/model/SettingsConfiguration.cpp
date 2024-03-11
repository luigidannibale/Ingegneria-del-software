#include "SettingsConfiguration.h"

Chessboard SettingsConfiguration::GetChessboard() {return chessboard;}
Pieces SettingsConfiguration::GetPieces() {return pieces;}

void SettingsConfiguration::SetChessboard(Chessboard chessboard) {this->chessboard = chessboard;}
void SettingsConfiguration::SetPieces(Pieces pieces) {this->pieces = pieces;}