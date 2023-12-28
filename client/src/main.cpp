#include "main.h"

class ChessClient : public wxApp {
public:
    virtual bool OnInit();
};

bool ChessClient::OnInit() {
    Controller* c = new Controller();

    // HomePage *frame = new HomePage("Gioco degli scacchi",wxPoint(10,20),wxSize(800,600));
    // frame->SetSize(800,600);
    // frame->Center();
    // frame->Show(true);
    
    return true;
}

wxIMPLEMENT_APP(ChessClient);