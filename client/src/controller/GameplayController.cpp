#include "GameplayController.h"
#include <string_view>
#include <map>
const char* FILES[] = {"87654321","12345678"};
const char* ROWS[] =  {"abcdefgh","hgfedcba"};
const bool MARK_CELLS = true;


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
    coordinates[0] = ROWS[isWhite? 0 : 1][colonna];
    coordinates[1] = FILES[isWhite? 0 : 1][riga];

    return new CellCoordinates(riga,colonna,coordinates);
}

chess::PieceGenType TypeToGenType(chess::PieceType piece){
    if(piece == chess::PieceType::NONE){
        std::cout << "Errore in TypeToGenType: mi hai passato NONE" << std::endl;
        return chess::PieceGenType::PAWN;
    }
    return static_cast<chess::PieceGenType>(std::pow(2, static_cast<int>(piece)));
}

//chess::PieceType GenTypeToType(chess::PieceGenType piece){
//    return static_cast<chess::PieceType>(static_cast<int>(piece));
//}

void GameplayController::markFeasible(chess::Move move){
    if (!MARK_CELLS)
        return;
    ChessboardView* chessboard = frame->GetChessboard();
    int f = static_cast<int>(move.to().file());
    int r = static_cast<int>(move.to().rank());
    f = FILES[isWhite? 1 : 0][f]-'1';
    r = FILES[isWhite? 0 : 1][r]-'1';

    chessboard->GetCell(r, f)->feasible->Show(true);
}
void GameplayController::unmarkFeasibles(){
    if (!MARK_CELLS)
        return;
    ChessboardView* chessboard = frame->GetChessboard();
    for (const auto& move: playableMoves) {
        int f = static_cast<int>(move.second.to().file());
        int r = static_cast<int>(move.second.to().rank());
        f = FILES[isWhite? 1 : 0][f]-'1';
        r = FILES[isWhite? 0 : 1][r]-'1';
        chessboard->GetCell(r, f)->feasible->Show(false);
    }
}
void printMove(chess::Move move){
    printf("E' stata giocata la mossa : ");
    std::cout << move << std::endl;
    //TOdo stampare la mossa in un pannello accanto la scacchiera
}

void GameplayController::UpdateChessboard() {
    std::string fen = board.getFen();

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
        if (clicked->piece==chess::PieceType::NONE){
            unmarkFeasibles();
        }
        std::string_view to {coordinates->coordinates};
        clickedCoord = nullptr;
        //Performando la mossa

        if(playableMoves.count(chess::Square(to))>0){
            chess::Move move = playableMoves.at(chess::Square(to));
            board.makeMove(move);
            chessboard->update(board.getFen());
            // chessboard->MovePiece(preC->row, preC->col, coordinates->row, coordinates->col);
            printMove(move);
            frame->ChangeTimer();
            unmarkFeasibles();
            playableMoves.clear();
            return;
        }
        else { // Se la cella cliccata non è nelle mosse possibili
            unmarkFeasibles();
            playableMoves.clear();
        }
    }
    //Click di selezione del pezzo da muovere (e calcolo delle mosse giocabili)
    if (clickedCoord == nullptr){
        Cell* clicked = chessboard->GetCell(coordinates->row, coordinates->col);
        clickedCoord = coordinates;
        if (clicked->piece==chess::PieceType::NONE) {
            clickedCoord = nullptr;
            return;
        }
        else{
            std::string_view s {coordinates->coordinates};
            std::cout << coordinates->coordinates << std::endl;
            unmarkFeasibles();
            playableMoves.clear();
            chess::movegen::legalmoves(moves, board, TypeToGenType(clicked->piece));
            for (const auto &move : moves) {
                if(move.from().operator==(chess::Square(s))) {
                    playableMoves.emplace(move.to(), move);
                    markFeasible(move);
                }
            }
            chessboard->SetPreFEN(board.getFen());
            if (moves.empty())
                printf("No moves available \n");
            return;
        }
    }
}