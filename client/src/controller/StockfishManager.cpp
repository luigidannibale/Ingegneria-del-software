#include "StockfishManager.h"

StockfishManager::StockfishManager(){
    initialize_stockfish();
}
StockfishManager::~StockfishManager() {
    close_stockfish();
}

void StockfishManager::close_stockfish() {
    std::cout << "Stockfish successfully closed" << std::endl;
    send_command("quit");
    close(write_fd);
    close(read_fd);
    waitpid(pid_stockfish, nullptr, 0);
}



// Function to communicate with Stockfish and get the evaluation score
std::string StockfishManager::run_commands(const std::vector<std::string>& commands) {

    // Write commands to Stockfish
    for (const auto& command : commands) {
        send_command(command);

    }

    // Read output from Stockfish
    std::string result;
    char buffer[128];

    ssize_t count;
    while ((count = read(out_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[count] = '\0';
        result += buffer;
    }
    // Wait for child process to finish
    return result;
}

// Funzione per inviare un comando a Stockfish
void StockfishManager::send_command(const std::string command) {
    write(write_fd, command.c_str(), command.size());
    write(write_fd, "\n", 1);
}
// Funzione per inizializzare Stockfish con i comandi UCI e isready
void StockfishManager::initialize_stockfish() {

    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) {
        throw std::runtime_error("Failed to create pipes");
    }

    pid_stockfish = fork();
    if (pid_stockfish == -1) {
        throw std::runtime_error("Failed to fork process");
    }

    if (pid_stockfish == 0) { // Processo figlio
        close(in_pipe[1]); // Chiudi l'estremità di scrittura della pipe di input
        close(out_pipe[0]); // Chiudi l'estremità di lettura della pipe di output

        dup2(in_pipe[0], STDIN_FILENO); // Reindirizza stdin alla pipe di input
        dup2(out_pipe[1], STDOUT_FILENO); // Reindirizza stdout alla pipe di output
        dup2(out_pipe[1], STDERR_FILENO); // Reindirizza stderr alla pipe di output

        execl(stockfish_path.c_str(), stockfish_path.c_str(), nullptr);
        _exit(1); // Esci se exec fallisce
    }

    // Processo padre
    close(in_pipe[0]);
    close(out_pipe[1]);

    write_fd = in_pipe[1];
    read_fd = out_pipe[0];

    try {
        send_command("uci");
        std::string uci_response = read_response();
        if (uci_response.find("uciok") == std::string::npos) {
            throw std::runtime_error("Failed to receive uciok from Stockfish");
        }
        send_command("isready");
        std::string isready_response = read_response();
        if (isready_response.find("readyok") == std::string::npos) {
            throw std::runtime_error("Failed to receive readyok from Stockfish");
        }
        std::cout << "Stockfish initialized successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in initializing stockfish : " << e.what() << std::endl;
        throw;
    }
}

// Funzione per leggere la risposta di Stockfish
std::string StockfishManager::read_response() {
    std::string result;
    std::array<char, 128> buffer;
    ssize_t count;

    fd_set set;
    struct timeval timeout;

    // Inizializza il set di file descriptor
    FD_ZERO(&set);
    FD_SET(read_fd, &set);

    // Timeout di 2 secondi
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    while (true) {
        // Controlla se ci sono dati disponibili per la lettura
        int rv = select(read_fd + 1, &set, NULL, NULL, &timeout);
        if (rv == -1) {
            throw std::runtime_error("select() failed");
        } else if (rv == 0) {
            // Timeout raggiunto
            break;
        } else {
            // Dati disponibili per la lettura
            count = read(read_fd, buffer.data(), buffer.size() - 1);
            if (count > 0) {
                buffer[count] = '\0';
                result += buffer.data();
                /*// Controlla se abbiamo ricevuto il comando 'bestmove'
                if (result.find("bestmove") != std::string::npos) {
                    break;
                }*/
            }
            else
                break;
        }
    }

    return result;
}

// Funzione per ottenere una valutazione dalla posizione
std::string StockfishManager::get_bestmove(const std::string fen, int depth) {
    std::vector<std::string> commands = {
            "position fen " + fen,
            "go depth " + std::to_string(depth),
    };

    for (const auto& command : commands) {
        send_command(command);
    }
    std::string result = read_response();
    int t = result.find("bestmove ");
    size_t start = t + 9; // "bestmove " is 9 characters long
    size_t end = result.find(" ponder", start);
    std::string move = result.substr(start, end - start);
    return move;
}

float StockfishManager::get_eval(const std::string fen){
    std::vector<std::string> commands = {
            "position fen " + fen,
            "eval " ,
    };

    for (const auto& command : commands) {
        send_command(command);
    }
    std::string result = read_response();
    int t = result.find("Final evaluation       ");
    size_t start = t + 23; // "Final evaluation       " is 23 characters long
    size_t end = result.find(" (", start);
    std::string eva = result.substr(start, end - start);
    std::cout<<"Eval is "<<eva<<std::endl;
}