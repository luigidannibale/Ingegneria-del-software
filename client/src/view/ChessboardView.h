#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <string>
#include "ImageFuncions.h"
#include "../lib.h"
#include "../model/chess_pachage/chess.hpp"

enum class Piece{
    Knight = 2,
    Bishop = 4,
    Pawn = 1,
    Rook = 8,
    Queen = 16,
    King = 32,
    Empty
};


class Cell{
public:
    Cell(wxStaticBitmap* bitmap, Piece piece) {
        this->bitmap = bitmap;
        this->piece = piece;
        feasible = new wxStaticBitmap(bitmap->GetParent(), wxID_ANY, img::GetImage("../resources/img/step64_3.png"));
        bitmap->Show(false);
        feasible->Show(false);
        bitmap->Show(true);
    }

    void Move(int x, int y) {
        bitmap->Move(x, y);
        feasible->Move(x,y);
        //feasible->Show(false);
    }
    wxStaticBitmap* feasible ;
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
