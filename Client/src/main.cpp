#include "main.hpp"

class ChessClient : public wxApp {
public:
    virtual bool OnInit();
};

bool ChessClient::OnInit() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    HomeController* c = new HomeController();

    return true;
}

wxIMPLEMENT_APP(ChessClient);