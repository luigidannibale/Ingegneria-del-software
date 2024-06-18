#ifndef SETTINGS_H
#define SETTINGS_H


enum ChessboardStyle{
    Default = 1,
    Wood = 2,
    Blue = 3
};

enum PiecesStyle{
    Style1 = 1,
    Style2 = 2,
    Style3 = 3
};


class Settings {
    Settings(ChessboardStyle, PiecesStyle);
    ChessboardStyle GetChessboardStyle();
    PiecesStyle GetPiecesStyle();
    void SetChessboardStyle(enum ChessboardStyle);
    void SetPiecesStyle(enum PiecesStyle);

private:
    enum ChessboardStyle chessboardStyle;
    enum PiecesStyle piecesStyle;
};


#endif //SETTINGS_H
