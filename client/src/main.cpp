#include "main.h"

class ChessClient : public wxApp {
public:
    virtual bool OnInit();
};

bool ChessClient::OnInit() {
    HomeController* c = new HomeController();

    return true;
}

wxIMPLEMENT_APP(ChessClient);