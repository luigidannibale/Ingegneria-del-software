#ifndef CHESSBOARDVIEW_H
#define CHESSBOARDVIEW_H

#include <string>
#include <cctype>
#include <sstream>
#include "ImageFuncions.hpp"
#include "../lib.hpp"
#include "../model/chess_pachage/chess.hpp"

class Cell{
public:
    Cell(wxStaticBitmap* bitmap, chess::PieceType piece,float scal) {
        this->bitmap = bitmap;
        this->piece = piece;
        feasible = new wxStaticBitmap(bitmap->GetParent(), wxID_ANY, img::GetImageAndScale("step64_3.png",scal));
        bitmap->Show(false);
        feasible->Show(false);
        bitmap->Show(true);
    }

    void Move(int x, int y) {
        bitmap->Move(x, y);
        feasible->Move(x,y);
    }
    wxStaticBitmap* feasible ;
    wxStaticBitmap* bitmap;
    chess::PieceType piece;
};

class ChessboardView {
public:
    ChessboardView(wxStaticBitmap*, std::string, float, float, bool,float);
    wxStaticBitmap* GetBoard();
    Cell* GetCell(int, int);
    float GetCellDimension(){return cellDimension;}
    void MovePiece(int, int, int, int);
    void SetPreFEN(std::string);
    void update(std::string);
private:
    bool white = true;
    float moveX;
    float moveY;

    float cellDimension = 80;

    std::string preFEN;
    std::string postFEN;

    wxImage boardImg;
    wxStaticBitmap* boardBitmap;

    wxImage pawnImgs[2];
    wxImage kingImgs[2];
    wxImage queenImgs[2];
    wxImage bishopImgs[2];
    wxImage knightImgs[2];
    wxImage rookImgs[2];

    Cell* boardCellsBitmap[8][8];
    void CreatePieces(Cell**, int, int,float);

};


#endif //CHESSBOARDVIEW_H
