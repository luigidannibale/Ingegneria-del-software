#include "GameplayController.h"

const char* FILES[] = {"87654321","12345678"};
const char* ROWS[] =  {"abcdefgh","hgfedcba"};
const bool MARK_CELLS = true;

GameplayController::GameplayController(GameOptions* options) {
    gameManager = new GameManager(options);
    frame = new GameplayFrame(gameManager->isWhite(), options);
    frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
}



GameplayController::~GameplayController(){

}

CellCoordinates* GetPosition(wxPoint pointClicked, bool isWhite, float cellDim){
    /*
    ----------------------------------------------1
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
    int riga = pointClicked.y / cellDim;
    int colonna = pointClicked.x / cellDim;

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


void GameplayController::markFeasible(chess::Move move){
    if (!MARK_CELLS)
        return;
    ChessboardView* chessboard = frame->GetChessboard();
    int f = static_cast<int>(move.to().file());
    int r = static_cast<int>(move.to().rank());
    f = FILES[gameManager->isWhite()? 1 : 0][f]-'1';
    r = FILES[gameManager->isWhite()? 0 : 1][r]-'1';
    chessboard->GetCell(r, f)->feasible->Show(true);
}

void GameplayController::unmarkFeasibles(){
    if (!MARK_CELLS)
        return;
    ChessboardView* chessboard = frame->GetChessboard();
    for (const auto& move: playableMoves) {
        int f = static_cast<int>(move.second.to().file());
        int r = static_cast<int>(move.second.to().rank());
        f = FILES[gameManager->isWhite()? 1 : 0][f]-'1';
        r = FILES[gameManager->isWhite()? 0 : 1][r]-'1';
        chessboard->GetCell(r, f)->feasible->Show(false);
    }
}
void printMove(chess::Move move){
    printf("E' stata giocata la mossa : ");
    std::cout << move << std::endl;
    //TOdo stampare la mossa in un pannello accanto la scacchiera
}

void GameplayController::makeMove(std::string_view to) {
    chess::Move move = playableMoves.at(chess::Square(to));
    chess::Board c = gameManager->GetBoard();
    c.makeMove(move);
    gameManager->updateBoard(c);
    frame->GetChessboard()->update(gameManager->GetBoard().getFen());
    // chessboard->MovePiece(preC->row, preC->col, coordinates->row, coordinates->col);
    printMove(move);
    frame->ChangeTimer();
    unmarkFeasibles();
    playableMoves.clear();
    gameManager->swapTurn();
    
}

void GameplayController::ClickBoard(wxMouseEvent& event) {
    /*if (!gameManager->playerCanPlay())
        return;*/
    CellCoordinates* coordinates = GetPosition(event.GetPosition(),gameManager->isWhite(),frame->GetChessboard()->GetCellDimension());
    ChessboardView* chessboard = frame->GetChessboard();
    chess::Board board = gameManager->GetBoard();
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
            makeMove(to);
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

            chess::movegen::legalmoves(moves, board);

            for (const auto &move : moves) {
                std::cout << chess::uci::moveToUci(move) <<" "<< perft(board,1);<<" " <<std::endl;

            }
            chess::movegen::legalmoves(moves, board, TypeToGenType(clicked->piece));
            for (const auto &move : moves) {
                //std::cout<< move.to().file().operator std::string() << move.to().rank().operator std::string() << " from " << move.from().file().operator std::string()<<move.from().rank().operator std::string()<< " - "<<move.score()<<std::endl;
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