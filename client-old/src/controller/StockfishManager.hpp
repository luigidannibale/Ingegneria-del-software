#ifndef STOCKFISHMANAGER_H
#define STOCKFISHMANAGER_H

#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <array>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <array>
#include <atomic>
#include <thread>


class StockfishManager {
public:
    StockfishManager();
    ~StockfishManager();
    std::string run_commands(const std::vector<std::string>&);
    std::string get_bestmove(const std::string, int depth = 10);
    float get_eval(const std::string);
    void close_stockfish();
private:
    void initialize_stockfish();

    void send_command(std::string);
    std::string read_response();
    const std::string stockfish_path = "./stockfish";
    int in_pipe[2];
    int out_pipe[2];

    int read_fd;
    int write_fd;
    pid_t pid_stockfish;

    bool closed = false;
    std::atomic<bool> isCommandRunning;
};

#endif //STOCKFISHMANAGER_H
