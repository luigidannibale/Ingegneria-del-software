
#include "GameplayFrame.h"

GameplayFrame::GameplayFrame(): wxFrame(NULL, wxID_ANY, wxString("Gioca la partita vinci la fatica"), wxPoint(0,0), wxSize(0,0), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {
    SetSize(1200,800);
    Center();
    Show();
    SetBackgroundColour(wxColour(118,150,86));

    wxImage boardImg = img::GetImage(IMGPATH + "chessboard-blue.png");
    chessboard = new ChessboardView(new wxStaticBitmap(this, wxID_ANY, wxBitmap(boardImg)),
                                    "icons/", chessX, chessY);

    opponentText = new wxStaticText(this, wxID_ANY,wxString("Avversario cattivo"));
    opponentText->Move(1000,50);
    userText = new wxStaticText(this, wxID_ANY,wxString("Io"));
    userText->Move(1000,700);
}



void GameplayFrame::StartGame() {
}

wxStaticBitmap* GameplayFrame::GetBoard() { return chessboard->GetBoard(); }
ChessboardView* GameplayFrame::GetChessboard() {return chessboard;}