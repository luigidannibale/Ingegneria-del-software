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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <array>
#include <fcntl.h>

// Function to communicate with Stockfish and get the evaluation score
std::string run_stockfish(const std::vector<std::string>& commands) {
    int in_pipe[2];
    int out_pipe[2];

    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) {
        throw std::runtime_error("Failed to create pipes");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Failed to fork process");
    }

    if (pid == 0) { // Child process
        close(in_pipe[1]); // Close write end of input pipe
        close(out_pipe[0]); // Close read end of output pipe

        dup2(in_pipe[0], STDIN_FILENO); // Redirect stdin to read end of input pipe
        dup2(out_pipe[1], STDOUT_FILENO); // Redirect stdout to write end of output pipe
        dup2(out_pipe[1], STDERR_FILENO); // Redirect stderr to write end of output pipe

        execl("./stockfish", "./stockfish", NULL);
        _exit(1); // If exec fails
    }

    // Parent process
    close(in_pipe[0]); // Close read end of input pipe
    close(out_pipe[1]); // Close write end of output pipe

    // Write commands to Stockfish
    for (const auto& command : commands) {
        write(in_pipe[1], command.c_str(), command.size());
        write(in_pipe[1], "\n", 1);
    }
    close(in_pipe[1]); // Close write end after writing all commands

    // Read output from Stockfish
    std::string result;
    char buffer[128];
    ssize_t count;
    while ((count = read(out_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[count] = '\0';
        result += buffer;
    }
    close(out_pipe[0]); // Close read end after reading all output

    // Wait for child process to finish
    waitpid(pid, nullptr, 0);

    return result;
}

// Funzione per inviare un comando a Stockfish e leggere la risposta
std::string send_command(int write_fd, int read_fd, const std::string& command) {
    // Invia il comando a Stockfish
    write(write_fd, command.c_str(), command.size());
    write(write_fd, "\n", 1);

    // Leggi la risposta da Stockfish
    std::string result;
    std::array<char, 128> buffer;
    ssize_t count;
    while ((count = read(read_fd, buffer.data(), buffer.size() - 1)) > 0) {
        buffer[count] = '\0';
        result += buffer.data();
        if (result.find("uciok") != std::string::npos || result.find("readyok") != std::string::npos) {
            break;
        }
    }
    return result;
}

// Funzione per inizializzare Stockfish con i comandi UCI e isready
void initialize_stockfish(const std::string& stockfish_path, int &write_fd, int &read_fd) {
    int in_pipe[2];
    int out_pipe[2];

    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) {
        throw std::runtime_error("Failed to create pipes");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Failed to fork process");
    }

    if (pid == 0) { // Processo figlio
        close(in_pipe[1]); // Chiudi l'estremità di scrittura della pipe di input
        close(out_pipe[0]); // Chiudi l'estremità di lettura della pipe di output

        dup2(in_pipe[0], STDIN_FILENO); // Reindirizza stdin alla pipe di input
        dup2(out_pipe[1], STDOUT_FILENO); // Reindirizza stdout alla pipe di output
        dup2(out_pipe[1], STDERR_FILENO); // Reindirizza stderr alla pipe di output

        execl(stockfish_path.c_str(), stockfish_path.c_str(), nullptr);
        _exit(1); // Esci se exec fallisce
    }

    // Processo padre
    close(in_pipe[0]); // Chiudi l'estremità di lettura della pipe di input
    close(out_pipe[1]); // Chiudi l'estremità di scrittura della pipe di output

    write_fd = in_pipe[1];
    read_fd = out_pipe[0];

    try {
        // Invia il comando UCI e aspetta uciok
        std::string uci_response = send_command(write_fd, read_fd, "uci");
        if (uci_response.find("uciok") == std::string::npos) {
            throw std::runtime_error("Failed to receive uciok from Stockfish");
        }

        // Invia il comando isready e aspetta readyok
        std::string isready_response = send_command(write_fd, read_fd, "isready");
        if (isready_response.find("readyok") == std::string::npos) {
            throw std::runtime_error("Failed to receive readyok from Stockfish");
        }

        std::cout << "Stockfish initialized successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

// Funzione per ottenere una valutazione dalla posizione
std::string get_evaluation(const std::string& fen) {
    std::vector<std::string> commands = {
            "position fen " + fen,
            "go depth 10"
    };
    return run_stockfish(commands);
}


void evalBoard(chess::Board board){

    // Percorso all'eseguibile di Stockfish
    std::string stockfish_path = "./stockfish";
    int write_fd, read_fd;
    try {
        initialize_stockfish(stockfish_path, write_fd, read_fd);
        // Esempio di posizione FEN
        std::string fen = board.getFen();
        std::string output = get_evaluation(fen);

        std::cout << "Stockfish output:\n" << output << std::endl;

        // Parse the output to find the evaluation score
        /*size_t score_pos = output.find("score cp");
        if (score_pos != std::string::npos) {
            size_t start = score_pos + 9; // "score cp " is 9 characters long
            size_t end = output.find(" ", start);
            std::string score_str = output.substr(start, end - start);
            int score = std::stoi(score_str);
            std::cout << "Evaluation score: " << score << std::endl;
        } else {
            std::cerr << "Failed to parse Stockfish output" << std::endl;
        }*/
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
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
    evalBoard(c);
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

            /*for (const auto &move : moves) {
                std::cout << chess::uci::moveToUci(move) <<" "<< perft(board,1);<<" " <<std::endl;

            }*/
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