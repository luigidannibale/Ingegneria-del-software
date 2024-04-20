#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <wx-3.0/wx/wx.h>
#include <string>
#include "ImageFuncions.h"


enum class Piece{
    Knight,
    Bishop,
    Pawn,
    Rook,
    Queen,
    King,
    Empty
};


class Cell{
public:
    Cell(wxStaticBitmap* bitmap, Piece piece) {
        this->bitmap = bitmap;
        this->piece = piece;
    }

    void Move(int x, int y) {
        bitmap->Move(x, y);
    }

    wxStaticBitmap* bitmap;
    Piece piece;
};

class ChessboardView {
public:
    ChessboardView(wxStaticBitmap*, std::string, int, int, bool);
    wxStaticBitmap* GetBoard();
    Cell* GetCell(int, int);
    void MovePiece(int, int, int, int);
private:
    std::string const IMGPATH = "../resources/img/";
    bool white = true;
    int moveX;
    int moveY;

    const int cellDimension = 80;


    wxImage boardImg;
    wxStaticBitmap* boardBitmap;

    wxImage pawnImgs[2];
    wxImage kingImgs[2];
    wxImage queenImgs[2];
    wxImage bishopImgs[2];
    wxImage knightImgs[2];
    wxImage rookImgs[2];

    Cell* boardCellsBitmap[8][8];
    void CreatePieces(Cell**, int, int);
};




#endif //CHESSBOARDVIEW_H
