#include "GameplayController.h"

GameplayController::GameplayController(GameOptions* options) {

    if (options->GetStartSide() == StartSide::Casual) {
        int randomInt = std::rand() % 2;
        isWhite = static_cast<bool>(randomInt);
    }
    else
        isWhite = options->GetStartSide() == StartSide::White;

    frame = new GameplayFrame(isWhite);

    frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
}


GameplayController::~GameplayController(){

}

CellCoordinates* GetPosition(wxPoint pointClicked, bool isWhite){
    /*
    ----------------------------------------------
    |riga 0 colonna 0    ----    riga 0 colonna 7
    |
    |
    |
    |
    |
    |
    |riga 7 colonna 0    ----    riga 7 colonna 7
    ----------------------------------------------
    */
    int riga = pointClicked.y / 80;
    int colonna = pointClicked.x / 80;
    const char* y[] = {"87654321","12345678"};
    const char* x[] =  {"abcdefgh","hgfedcba"};
    char coordinates[]   = "a-1";
    coordinates[0] = x[isWhite? 0 : 1][colonna];
    coordinates[2] = y[isWhite? 0 : 1][riga];

    return new CellCoordinates(riga,colonna,coordinates);
}

void GameplayController::ClickBoard(wxMouseEvent& event) {
    CellCoordinates* coordinates = GetPosition(event.GetPosition(),isWhite);
    ChessboardView* chessboard = frame->GetChessboard();

    // TODO: aggiungere controllo turno

    if (clickedCoord == nullptr){
        Cell* clicked = chessboard->GetCell(coordinates->row, coordinates->col);
        clickedCoord = coordinates;
        if (clicked->piece==Piece::Empty) {
            clickedCoord = nullptr;
            return;
        }
    }
    else{
        //TODO : check it is my turn
        CellCoordinates* preC = clickedCoord;
        Cell* clicked = chessboard->GetCell(coordinates->row, coordinates->col);
        //TODO: check action is legal

        chessboard->MovePiece(preC->row, preC->col, coordinates->row, coordinates->col);

        clickedCoord = nullptr;
    }
}