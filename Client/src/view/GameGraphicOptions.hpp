#ifndef GAMEGRAPHICOPTIONS_H
#define GAMEGRAPHICOPTIONS_H

enum class BoardStyle : int
{
    brown = 1,
    blue,
    black,
};
enum class PiecesStyle : int
{
    neo = 1,
    pixel,
};

class GameGraphicOptions
{
public:
    GameGraphicOptions() {}
    GameGraphicOptions(BoardStyle, PiecesStyle);
    BoardStyle GetBoardStyle();
    PiecesStyle GetPiecesStyle();

    void SetBoardStyle(BoardStyle);
    void SetPiecesStyle(PiecesStyle);

private:
    BoardStyle boardStyle;
    PiecesStyle piecesStyle;
};

#endif // GAMEGRAPHICOPTIONS_H
