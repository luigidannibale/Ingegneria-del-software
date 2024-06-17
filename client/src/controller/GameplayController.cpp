#include "GameplayController.h"

const char* FILES[] = {"87654321","12345678"};
const char* ROWS[] =  {"abcdefgh","hgfedcba"};
const bool MARK_CELLS = true;
bool Stock_init = false;

GameplayController::GameplayController(GameOptions* options,GameGraphicOptions* graphicOptions, RedisManager* redisManager, std::string channel) {
    gameManager = new GameManager(options);
    this->redisManager = redisManager;
    this->channel = channel;
    std::cout << "Channel is " << channel << std::endl;
    auto f = [this]() {
        if (!gameManager->isAgainstHuman()) {
            gameManager->StartStockfish();
        } else {
            if (!gameManager->playerCanPlay()) {
                this->redisManager->SubscribeToChannel(this->channel.c_str());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

        gameClosed = false;
        frame->CallAfter([this]() {
            if (gameClosed)
                return;

            frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
            frame->HideTransparentPanel();
            frame->StartTimer();
            if (!gameManager->playerCanPlay()) {
                if (!gameManager->isAgainstHuman())
                    AsyncComputerMove();
                else
                    AsyncHumanMove();
            }
        });
    };

    std::thread t(f);
    t.detach();

    frame = new GameplayFrame(gameManager->isWhite(), options, graphicOptions);
    frame->Bind(wxEVT_CLOSE_WINDOW, &GameplayController::OnClose, this);
    frame->GetChessboard()->SetPreFEN(gameManager->GetBoard().getFen());
    frame->Bind(wxEVT_TIMER, &GameplayController::UpdateTime, this);
}


GameplayController::~GameplayController(){

}

void GameplayController::OnClose(wxCloseEvent& event){
    std::cout<<"Chiudo gameplaycontroller"<<std::endl;

    // if (!gameEnded && gameManager->isAgainstHuman()) {
    //     std::string message = "quit";
    //     if (redisManager->PublishToChannel(channel.c_str(), message.c_str())) {
    //         std::cout << "Published message " << message << " to channel " << channel << std::endl;
    //     } else {
    //         std::cerr << "Failed to publish message " << message << " to channel " << channel << std::endl;
    //     }

    //     if (redisManager->UnsubscribeFromChannel(channel.c_str())) {
    //         std::cout << "Unsubscribed from channel " << channel << std::endl;
    //     } else {
    //         std::cerr << "Failed to unsubscribe from channel " << channel << std::endl;
    //     }
    // }

    // frame->StopUpdateTimer();
    // frame->UpdateTransparentPanel("Game Closed");
    // frame->ShowTransparentPanel();
    gameManager->~GameManager();
    gameClosed = true;
    frame->~GameplayFrame();
}

void GameplayController::UpdateTime(wxTimerEvent& event) {
    int res = frame->UpdateTime(event);
    if (res != 0) { // A timer ended
        if (gameManager->isWhite()) {
            if (res == 1) { // White timer ended
                frame->UpdateTransparentPanel("You Lose!");
                frame->ShowTransparentPanel();
            } else if (res == 2) { // Black timer ended
                frame->UpdateTransparentPanel("You Win!");
                frame->ShowTransparentPanel();
            }
        } else {
            if (res == 1) {
                frame->UpdateTransparentPanel("You Win!");
                frame->ShowTransparentPanel();
            } else if (res == 2) {
                frame->UpdateTransparentPanel("You Lose!");
                frame->ShowTransparentPanel();
            }
        
        }
    }
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
void GameplayController::printMove(chess::Piece piece,chess::Move move, bool capture, bool ischeck){
    std::cout << "E' stata giocata la mossa : " << move << std::endl;
    std::string p = std::string(piece);
    std::string item = (p != "p" && p != "P" ? p : "") + std::string(move.to());
    if (move.typeOf() == move.CASTLING){
        if (move.to()==chess::Square("h8") || move.to()==chess::Square("h1"))
            item = "O-O";
        if (move.to()==chess::Square("a8") || move.to()==chess::Square("a1"))
            item = "O-O-O";
    }
    else if (capture)
        item = (p != "p" && p != "P" ? p : "") + std::string(move.from()) + "x" + std::string(move.to());

    auto gameOver = gameManager->GetBoard().isGameOver().second;
    if (gameOver != chess::GameResult::NONE && gameOver != chess::GameResult::DRAW)
        item = item+"#";
    else if (ischeck)
        item = item+"+";
    frame->AddMoveToList(item);
}

bool GameplayController::CheckCheckmate(){
    std::pair<chess::GameResultReason, chess::GameResult> gameOver = gameManager->GetBoard().isGameOver();
    chess::GameResult result = gameOver.second;
    chess::Color sideToMove = gameManager->GetBoard().sideToMove();
    if (result != chess::GameResult::NONE && (
        (sideToMove == chess::Color::BLACK && gameManager->isWhite()) || 
        (sideToMove == chess::Color::WHITE && !gameManager->isWhite())))
        result = chess::GameResult::WIN;
    switch(result) {
        case chess::GameResult::WIN:
            frame->UpdateTransparentPanel("You Win!");
            frame->ShowTransparentPanel();
            gameEnded = true;
            break;
        case chess::GameResult::LOSE:
            frame->UpdateTransparentPanel("You Lose!");
            frame->ShowTransparentPanel();
            gameEnded = true;
            break;
        case chess::GameResult::DRAW:
            frame->UpdateTransparentPanel("Draw!");
            frame->ShowTransparentPanel();
            gameEnded = true;
            break;
        case chess::GameResult::NONE:
            gameEnded = false;
            break;
    }
    return gameEnded;
}

void GameplayController::AsyncComputerMove() {
    auto f = [this]() {
        chess::Move move = chess::Move();
        int i = 0;
        while (move == chess::Move()) {
            move = gameManager->GetBestMove();
            if (i == 10)
                return;
        }
        chess::Board c = gameManager->GetBoard();

        chess::Piece piece = c.at(chess::Square(move.from()));

        bool isCapture = gameManager->GetBoard().isCapture(move);

        c.makeMove(move);
        gameManager->swapTurn();
        gameManager->updateBoard(c);
        bool inCheck = gameManager->GetBoard().inCheck();
        int random = std::rand() % MS_STOCKFISH_DELAY;
        std::cout << "Sleeping for " << random << " milliseconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(random));
        if (gameClosed)
            return;

        std::string string_move = chess::uci::moveToUci(move);
        frame->CallAfter([this, piece, move, isCapture, inCheck]() {
            if (gameClosed)
                return;
            frame->GetChessboard()->update(gameManager->GetBoard().getFen());
            printMove(piece,move, isCapture, inCheck);
            //frame->AddMoveToList(piece, move);
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

void GameplayController::AsyncHumanMove() {
    auto f = [this]() {
        std::string response = redisManager->WaitResponse();
        std::string delimiter = ":";
        size_t pos = response.find(delimiter);
        std::string opponent_move = response.substr(0, pos);
        std::string fen = response.substr(pos + 1);
        std::cout << "Received move " << opponent_move << " from opponent" << std::endl;

        chess::Move move = chess::uci::uciToMove(gameManager->GetBoard(), opponent_move);
        chess::Board c = gameManager->GetBoard();
        chess::Piece piece = c.at(chess::Square(move.from()));

        bool isCapture = gameManager->GetBoard().isCapture(move);

        c.makeMove(move);
        gameManager->swapTurn();
        gameManager->updateBoard(c);

        bool inCheck = gameManager->GetBoard().inCheck();
        frame->CallAfter([this, piece, move, isCapture, inCheck]() {
            if (gameClosed)
                return;
            frame->GetChessboard()->update(gameManager->GetBoard().getFen());
            printMove(piece,move, isCapture, inCheck);
            //frame->AddMoveToList(piece, move);
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

void GameplayController::PublishHumanMove(std::string move) {
    if (gameClosed)
        return;

    std::cout << "The channel is " << channel << std::endl;

    if (redisManager->UnsubscribeFromChannel(channel.c_str())) {
        std::cout << "Unsubscribed from channel " << channel << std::endl;
    } else {
        std::cerr << "Failed to unsubscribe from channel " << channel << std::endl;
        return;
    }

    if (redisManager->PublishToChannel(channel.c_str(), move.c_str())) {
        std::cout << "Published move " << move << " to channel " << channel << std::endl;
    } else {
        std::cerr << "Failed to publish move " << move << " to channel " << channel << std::endl;
        return;
    }

    if (redisManager->SubscribeToChannel(channel.c_str())) {
        std::cout << "Subscribed to channel " << channel << std::endl;
    } else {
        std::cerr << "Failed to subscribe to channel " << channel << std::endl;
        return;
    }
}

void GameplayController::makeMove(std::string_view to) {
    chess::Move move = playableMoves.at(chess::Square(to));
    chess::Board c = gameManager->GetBoard();
    chess::Piece piece = c.at(chess::Square(move.from()));

    bool isCapture = gameManager->GetBoard().isCapture(move);
    c.makeMove(move);
    gameManager->updateBoard(c);
    frame->GetChessboard()->update(gameManager->GetBoard().getFen());
    bool inCheck = gameManager->GetBoard().inCheck();
    // chessboard->MovePiece(preC->row, preC->col, coordinates->row, coordinates->col);
    printMove(piece,move,isCapture,inCheck);
    std::string string_move = chess::uci::moveToUci(move);
    //frame->AddMoveToList(piece, move);

    unmarkFeasibles();
    playableMoves.clear();

    if (gameManager->isAgainstHuman())
        PublishHumanMove(string_move);

    if (CheckCheckmate()) {
        frame->StopUpdateTimer();
        return;
    }

    if (!gameManager->isAgainstHuman())
        AsyncComputerMove();
    else
        AsyncHumanMove();

    gameManager->swapTurn();
    frame->ChangeTimer();

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