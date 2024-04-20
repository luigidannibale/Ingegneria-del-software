
#include "GameplayFrame.h"

GameplayFrame::GameplayFrame(bool isWhite): wxFrame(NULL, wxID_ANY, wxString("Gioca la partita vinci la fatica"), wxPoint(0,0), wxSize(0,0), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {
    SetSize(1200,800);
    Center();
    Show();
    SetBackgroundColour(wxColour(118,150,86));

    wxImage boardImg = img::GetImage(IMGPATH + "chessboard-blue.png");
    chessboard = new ChessboardView(new wxStaticBitmap(this, wxID_ANY, wxBitmap(boardImg)),
                                    "icons/", chessX, chessY, isWhite);

    opponentText = new wxStaticText(this, wxID_ANY,wxString("Avversario cattivo"));
    opponentText->Move(1000,50);
    userText = new wxStaticText(this, wxID_ANY,wxString("Io"));
    userText->Move(1000,700);

    const char* labelY[] = {"87654321","12345678"};
    const char* labelX[] =  {"ABCDEFGH","HGFEDCBA"};

    wxFont font(wxFontInfo(14).Family(wxFONTFAMILY_MODERN));
    wxStaticText* prova = new wxStaticText(this, wxID_ANY, wxString("Prova"));
    wxSize fontSize = prova->GetFont().GetPixelSize();

    int index = isWhite?0:1;
    for (int i = 0; i < 8; i ++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString(labelY[index][i]));
        label->SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessboard->GetBoard()->GetSize().GetWidth() + chessX+5, chessY + i * 80 + (40 + fontSize.GetHeight()) / 2);
    }
    for (int i = 0; i < 8; i ++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString(labelX[index][i]));
        label-SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessX+ i*80, 25);
    }
}



void GameplayFrame::StartGame() {
}

wxStaticBitmap* GameplayFrame::GetBoard() { return chessboard->GetBoard(); }
ChessboardView* GameplayFrame::GetChessboard() {return chessboard;}