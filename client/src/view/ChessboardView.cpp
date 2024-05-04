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
        boardCellsBitmap[1][i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(pawnImgs[index])), chess::PieceType::PAWN);
        boardCellsBitmap[1][i]->Move(cellDimension*(i) + moveX,cellDimension+moveY);
    }
    CreatePieces(boardCellsBitmap[0],index,0);
    index = (index + 1) % 2;
    for (int i = 0; i < 8; ++i) {
        boardCellsBitmap[6][i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(pawnImgs[index])), chess::PieceType::PAWN);
        boardCellsBitmap[6][i]->Move(cellDimension*(i) + moveX, cellDimension*6+moveY);
    }

    CreatePieces(boardCellsBitmap[7],index,7);
for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            boardCellsBitmap[i][j] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxNullBitmap), chess::PieceType::NONE);
            boardCellsBitmap[i][j]->Move(moveX+j*cellDimension, moveY+i*cellDimension);
        }
    }

}



void ChessboardView::CreatePieces(Cell* rowToFill[], int index, int rpieces) {
    for (int i = 0; i < 8; i++) {
        int r = cellDimension*(rpieces) + moveY, c = cellDimension*(i) + moveX;
        switch (i) {
            case 0: case 7:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(rookImgs[index])),chess::PieceType::ROOK);
                break;
            case 1: case 6:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(knightImgs[index])),chess::PieceType::KNIGHT);
                break;
            case 2: case 5:
                rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(bishopImgs[index])),chess::PieceType::BISHOP);
                break;
            case 3:
                if(white)
                    rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(queenImgs[index])),chess::PieceType::QUEEN);
                else
                    rowToFill[i] = new Cell (new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(kingImgs[index])), chess::PieceType::KING);
                break;
            case 4:
                if(white)
                    rowToFill[i] = new Cell (new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(kingImgs[index])), chess::PieceType::KING);
                else
                    rowToFill[i] = new Cell(new wxStaticBitmap(boardBitmap->GetParent(), wxID_ANY, wxBitmap(queenImgs[index])),chess::PieceType::QUEEN);
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
    boardCellsBitmap[prerow][precolumn]->piece = chess::PieceType::NONE;
}

// Funziona
//  Non Translated FEN: rnbqkbnr/pppepppp/3p4/7p/p7/p6p/PPPPPPPP/RNBQKBNR w KQkq - 0 1
//      Translated FEN: rnbqkbnr/pppepppp/eeepeeee/eeeeeeep/peeeeeee/peeeeeep/PPPPPPPP/RNBQKBNR
std::string translateFEN(const std::string& fenString) {
    std::cout<<"TRANSLATING THIS FEN: "<<fenString<<std::endl;
    std::stringstream ss(fenString);
    std::string translatedFEN;

    char piece;
    int empty;
    int count = 0;
    while (ss >> piece) {
        if (isdigit(piece)) {
            empty = (piece - '0');
            for (int i = 0; i < empty; ++i) {
                translatedFEN += 'e'; // La 'e' rappresenta casella vuota
            }
            count += empty;
        } else if (piece == '/') { // Skippa gli slash
            continue;
        } else { // Caso in cui c'è un pezzo
            translatedFEN += piece;
            count ++;
        }
        if (count >= 64)
            break;
    }
    return translatedFEN.substr(0, 64);
}

void ChessboardView::update(std::string FEN){
    // "rnbqkbnr/pppepppp/3p4/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    // "rnbqkbnr/pppppppp/eeepeeee/eeeeeeee/eeeeeeee/eeeeeeee/PPPPPPPP/RNBQKBNR"
    postFEN = translateFEN(FEN);
    if (!white) {
        std::reverse(preFEN.begin(), preFEN.end());
        std::reverse(postFEN.begin(), postFEN.end());
    }
    std::cout << " PRE FEN " << preFEN << std::endl;
    std::cout << "POST FEN " << postFEN << std::endl;

    int length = postFEN.size();
//    int start = white ? 0 : 63;
//    int end = white ? length : 0;
    for (int i = 0; i < length; i++) {
        if (preFEN[i] == postFEN[i])
            continue; // Do nothing
        // Something changed
        //std::cout << "-" << i << " postFEN è " << postFEN[i] << std::endl;

        int row = i / 8;
        int column = i % 8;
        std::cout << "-----------------" << std::endl;
        std::cout << "INDICE: " << i << " postFEN è " << postFEN[i] << std::endl;
        std::cout << "RIGA: " << row << " COLONNA: " << column << std::endl;

        int index = isupper(postFEN[i]) ? 0 : 1;
        chess::PieceType piece;
        wxBitmap bitmap;
        switch(char(std::tolower(postFEN[i]))) {
            case 'p':
                piece = chess::PieceType::PAWN;
                bitmap = wxBitmap(pawnImgs[index]);
                //bitmap = boardCellsBitmap[1][0]->bitmap->GetBitmap();
                break;
            case 'b':
                piece = chess::PieceType::BISHOP;
                bitmap = wxBitmap(bishopImgs[index]);
                break;
            case 'n':
                piece = chess::PieceType::KNIGHT;
                bitmap = wxBitmap(knightImgs[index]);
                break;
            case 'r':
                piece = chess::PieceType::ROOK;
                bitmap = wxBitmap(rookImgs[index]);
                break;
            case 'q':
                piece = chess::PieceType::QUEEN;
                bitmap = wxBitmap(queenImgs[index]);
                break;
            case 'k':
                piece = chess::PieceType::KING;
                bitmap = wxBitmap(kingImgs[index]);
                break;
            case 'e':
                piece = chess::PieceType::NONE;
                bitmap = wxBitmap(1,1); // EMPTY BITMAP
                break;
        }

        boardCellsBitmap[row][column]->bitmap->SetBitmap(bitmap);
        boardCellsBitmap[row][column]->Move(column*cellDimension + moveX,row*cellDimension + moveY);
        boardCellsBitmap[row][column]->piece = piece;
    }
}

void ChessboardView::SetPreFEN(std::string FEN) {
    this->preFEN = translateFEN(FEN);
}
