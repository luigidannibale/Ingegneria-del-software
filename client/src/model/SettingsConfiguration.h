#ifndef SETTINGSCONFIGURATION_H
#define SETTINGSCONFIGURATION_H

enum class Chessboard {
    Black,
    Blue,
    Brown
};
enum class Pieces {
    p1,
    p2,
    p3
};

class SettingsConfiguration {
public:
    SettingsConfiguration(Chessboard,Pieces);
    void SetChessboard(Chessboard);
    void SetPieces(Pieces);
    Chessboard GetChessboard();
    Pieces GetPieces();
private:
    Chessboard chessboard;
    Pieces pieces;
};


#endif //SETTINGSCONFIGURATION_H
