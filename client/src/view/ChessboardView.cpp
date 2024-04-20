#include "ChessboardView.h"


ChessboardView::ChessboardView(wxStaticBitmap* chessboard, std::string iconsDir, int moveX, int moveY, bool isWhite) {
    this->boardBitmap = chessboard;
    this->moveX = moveX;
    this->moveY = moveY;
    this->white = isWhite;
    boardBitmap->Move(moveX, moveY);

    pawnImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whitePawn.png");
    pawnImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackPawn.png");

    kingImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whiteKing.png");
    kingImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackKing.png");

    queenImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whiteQueen.png");
    queenImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackQueen.png");

    rookImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whiteRook.png");
    rookImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackRook.png");

    bishopImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whiteBishop.png");
    bishopImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackBishop.png");

    knightImgs[0] = img::GetImage(IMGPATH + iconsDir + "/whiteKnight.png");
    knightImgs[1] = img::GetImage(IMGPATH + iconsDir + "/blackKnight.png");

    int index = white ? 1 : 0;
    for (int i = 0; i < 8; ++i) {
        boardCellsBitmap[1][i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(pawnImgs[index])), Piece::Pawn);
        boardCellsBitmap[1][i]->Move(cellDimension*(i) + moveX,cellDimension+moveY);
    }
    CreatePieces(boardCellsBitmap[0],index,0);
    index = (index + 1) % 2;
    for (int i = 0; i < 8; ++i) {
        boardCellsBitmap[6][i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(pawnImgs[index])), Piece::Pawn);
        boardCellsBitmap[6][i]->Move(cellDimension*(i) + moveX, cellDimension*6+moveY);
    }

    CreatePieces(boardCellsBitmap[7],index,7);
for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            boardCellsBitmap[i][j] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxNullBitmap), Piece::Empty);
            boardCellsBitmap[i][j]->Move(moveX+j*cellDimension, moveY+i*cellDimension);
        }
    }

}



void ChessboardView::CreatePieces(Cell* rowToFill[], int index, int rpieces) {
    for (int i = 0; i < 8; i++) {
        int r = cellDimension*(rpieces) + moveY, c = cellDimension*(i) + moveX;
        switch (i) {
            case 0: case 7:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(rookImgs[index])),Piece::Rook);
                break;
            case 1: case 6:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(knightImgs[index])),Piece::Knight);
                break;
            case 2: case 5:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(bishopImgs[index])),Piece::Bishop);
                break;
            case 3:
                if(white)
                    rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(queenImgs[index])),Piece::Queen);
                else
                    rowToFill[i] = new Cell (new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(kingImgs[index])), Piece::King);
                break;
            case 4:
                if(white)
                    rowToFill[i] = new Cell (new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(kingImgs[index])), Piece::King);
                else
                    rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(queenImgs[index])),Piece::Queen);
                break;
        }
        rowToFill[i]->Move(c,r);
    }
}

wxStaticBitmap* ChessboardView::GetBoard() { return boardBitmap; }

Cell* ChessboardView::GetCell(int row, int col) { return boardCellsBitmap[row][col]; }

void ChessboardView::MovePiece(int prerow, int precolumn, int row, int column) {
    
    // Moving the piece forward
    boardCellsBitmap[row][column]->bitmap->SetBitmap(boardCellsBitmap[prerow][precolumn]->bitmap->GetBitmap());
    boardCellsBitmap[row][column]->Move(column*cellDimension + moveX,row*cellDimension + moveY);
    boardCellsBitmap[row][column]->piece = boardCellsBitmap[prerow][precolumn]->piece;
    // Freeing the original outpost
    boardCellsBitmap[prerow][precolumn]->bitmap->SetBitmap(wxBitmap(1,1));
    boardCellsBitmap[prerow][precolumn]->piece = Piece::Empty;


}