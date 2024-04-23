#include "GameplayController.h"
#include <string_view>
#include <map>
const char* y[] = {"87654321","12345678"};
const char* x[] =  {"abcdefgh","hgfedcba"};

GameplayController::GameplayController(GameOptions* options) {

    if (options->GetStartSide() == StartSide::Casual) {
        int randomInt = std::rand() % 2;
        isWhite = static_cast<bool>(randomInt);
    }
    else
        isWhite = options->GetStartSide() == StartSide::White;

    frame = new GameplayFrame(isWhite, options->GetGameDurationInSeconds());
    frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
    board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

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

    std::string coordinates   = "a1";
    coordinates[0] = x[isWhite? 0 : 1][colonna];
    coordinates[1] = y[isWhite? 0 : 1][riga];

    return new CellCoordinates(riga,colonna,coordinates);
}

chess::PieceGenType toGenType(Piece piece){
    return static_cast<chess::PieceGenType>(static_cast<int>(piece));
}

Piece toPiece(chess::PieceGenType piece){
    return static_cast<Piece>(static_cast<int>(piece));
}
void markFeasible(chess::Move move, ChessboardView chessboard){
    int f = static_cast<int>(move.to().file());
    int r = static_cast<int>(move.to().rank());
    f = y[isWhite? 1 : 0][f]-'1';
    r = y[isWhite? 0 : 1][r]-'1';
    chessboard->GetCell(r, f)->feasible->Show(true);
}
void unmarkFeasibles(std::map<chess::Square,chess::Move>  playableMoves, ChessboardView chessboard){
    for (const auto& move: playableMoves) {
        int f = static_cast<int>(move.second.to().file());
        int r = static_cast<int>(move.second.to().rank());
        f = y[isWhite? 1 : 0][f]-'1';
        r = y[isWhite? 0 : 1][r]-'1';
        chessboard->GetCell(r, f)->feasible->Show(false);
    }
}
void GameplayController::ClickBoard(wxMouseEvent& event) {
    CellCoordinates* coordinates = GetPosition(event.GetPosition(),isWhite);
    ChessboardView* chessboard = frame->GetChessboard();

    // TODO: aggiungere controllo turno
    //Provando a fare il click di mossa (assumendo quindi ci sia stato un preclick di selezione di un pezzo da muovere)
    if (clickedCoord != nullptr){
        //TODO : check it is my turn
        CellCoordinates* preC = clickedCoord;
        Cell* clicked = chessboard->GetCell(coordinates->row, coordinates->col);
        //TODO: check action is legal
        std::string_view to {coordinates->coordinates};
        clickedCoord = nullptr;
        //Performando la mossa
        if(playableMoves.count(chess::Square(to))>0){
            chessboard->MovePiece(preC->row, preC->col, coordinates->row, coordinates->col);
            chess::Move move = playableMoves.at(chess::Square(to));
            board.makeMove(move);

            printf("Hai fatto la mossa : ");
            std::cout << move << std::endl;
            frame->ChangeTimer();
            unmarkFeasibles(playableMoves,chessboard);
            playableMoves.clear();
            return;
        }
    }
    //Click di selezione del pezzo da muovere (e calcolo delle mosse giocabili)
    if (clickedCoord == nullptr){
        Cell* clicked = chessboard->GetCell(coordinates->row, coordinates->col);
        clickedCoord = coordinates;
        if (clicked->piece==Piece::Empty) {
            clickedCoord = nullptr;
            return;
        }
        else{
            std::string_view s {coordinates->coordinates};
            std::cout << coordinates->coordinates << std::endl;
            unmarkFeasibles(playableMoves,chessboard);
            playableMoves.clear();
            chess::movegen::legalmoves(moves, board, toGenType(clicked->piece));
            for (const auto &move : moves) {
                if(move.from().operator==(chess::Square(s))) {
                    playableMoves.emplace(move.to(), move);
                    markFeasible(move,chessboard);
                }
            }
            if (moves.empty())
                printf("No moves available \n");
            return;
        }
    }
}