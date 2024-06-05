#include "GameplayController.h"

const char* FILES[] = {"87654321","12345678"};
const char* ROWS[] =  {"abcdefgh","hgfedcba"};
const bool MARK_CELLS = true;
bool Stock_init = false;

GameplayController::GameplayController(GameOptions* options) {
    gameManager = new GameManager(options);
    auto f = [this]() {
        gameManager->StartStockfish();

        frame->CallAfter([this]() {
            frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
            frame->HideTransparentPanel();
            frame->StartTimer();
            if (!gameManager->playerCanPlay()) {
                AsyncComputerMove();
            }
        });
    };

    std::thread t(f);
    t.detach();

    frame = new GameplayFrame(gameManager->isWhite(), options);
    frame->GetBoard()->Bind(wxEVT_CLOSE_WINDOW, &GameplayController::OnClose, this);
    frame->GetChessboard()->SetPreFEN(gameManager->GetBoard().getFen());
}


GameplayController::~GameplayController(){

}
void GameplayController::OnClose(wxCloseEvent& event){
    std::cout<<"Chiudo gamplaycontroller"<<std::endl;
    frame->~GameplayFrame();
    gameManager->~GameManager();
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

bool GameplayController::CheckCheckmate(){
    std::pair<chess::GameResultReason, chess::GameResult> gameOver = gameManager->GetBoard().isGameOver();
    chess::GameResult result = gameOver.second;
    switch(result) {
        case chess::GameResult::WIN:
            frame->UpdateTransparentPanel("You Win!");
            frame->ShowTransparentPanel();
            return true;
        case chess::GameResult::LOSE:
            frame->UpdateTransparentPanel("You Lose!");
            frame->ShowTransparentPanel();
            return true;
        case chess::GameResult::DRAW:
            frame->UpdateTransparentPanel("Draw!");
            frame->ShowTransparentPanel();
            return true;
        case chess::GameResult::NONE:
            return false;
    }
}

void GameplayController::AsyncComputerMove() {
    auto f = [this]() {
        chess::Move move = gameManager->GetBestMove();
        chess::Board c = gameManager->GetBoard();

        // std::cout << "Piece at " << move.from() << " is " << c.at(chess::Square(move.from())).type() << std::endl;
        // std::cout << "Piece at " << move.to() << " is " << c.at(chess::Square(move.to())).type() << std::endl;

        c.makeMove(move);
        gameManager->swapTurn();
        gameManager->updateBoard(c);

        int random = std::rand() % MS_STOCKFISH_DELAY;
        std::cout << "Sleeping for " << random << " milliseconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(random));

        frame->CallAfter([this]() {
            frame->GetChessboard()->update(gameManager->GetBoard().getFen());
            if (CheckCheckmate()) {
                frame->StopUpdateTimer();
                return;
            }
            frame->ChangeTimer();
        });
    };

    std::thread t(f);
    t.detach();
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

    if (CheckCheckmate()) {
        frame->StopUpdateTimer();
        return;
    }

    gameManager->swapTurn();
    AsyncComputerMove();
    // gameManager->makeComputerMove();
    // frame->ChangeTimer();
    // frame->GetChessboard()->update(gameManager->GetBoard().getFen());
}

void GameplayController::ClickBoard(wxMouseEvent& event) {
    if (!gameManager->playerCanPlay())
        return;
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

            chess::movegen::legalmoves(moves, board, TypeToGenType(clicked->piece));
            for (const auto &move : moves) {
                if(move.from().operator==(chess::Square(s))) {
                    playableMoves.emplace(move.to(), move);
                    markFeasible(move);
                }
            }
            // chessboard->SetPreFEN(board.getFen());
            if (moves.empty())
                printf("No moves available \n");
            return;
        }
    }
}