
#include "GameplayFrame.h"
#include <thread>
#include <chrono>

GameplayFrame::GameplayFrame(bool isWhite, int gameDurationsInSeconds): wxFrame(NULL, wxID_ANY, wxString("Gioca la partita vinci la fatica"), wxPoint(0,0), wxSize(0,0), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {
    SetSize(1100,750);
    Center();
    Show();
    SetBackgroundColour(wxColour(118,150,86));

    this->whiteSeconds = gameDurationsInSeconds;
    this->blackSeconds = gameDurationsInSeconds;

    // wxImage boardImg = img::GetImage(IMGPATH + "chessboard-blue.png");

    float scal = 1;
    wxImage boardImg = img::GetImageAndScale(IMGPATH + "chessboard-blue.png",scal);
    chessboard = new ChessboardView(new wxStaticBitmap(this, wxID_ANY, wxBitmap(boardImg)),"icons/", chessX, chessY, isWhite);

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
        // label->SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessboard->GetBoard()->GetSize().GetWidth() + chessX+5, chessY + i * 80 + (40 + fontSize.GetHeight()) / 2);
    }
    for (int i = 0; i < 8; i ++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString(labelX[index][i]));
        // label-SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessX+ i*80, 25);
    }

        /*wxDisplay display;
        // Get the geometry of the primary display (screen)
        wxRect screenRect = display.GetClientArea();
        // Retrieve the screen width
        int screenWidth = screenRect.GetWidth();*/

    std::string time = secondsToString(gameDurationsInSeconds);
    whiteTimerText = new wxStaticText(this, wxID_ANY, wxString(time));
    blackTimerText = new wxStaticText(this, wxID_ANY, wxString(time));
    whiteTimerText->Move(1000, 100);
    blackTimerText->Move(1000, 500);

    whiteTimer = new wxTimer(this, wxID_ANY);
    blackTimer = new wxTimer(this, wxID_ANY);
    Bind(wxEVT_TIMER, &GameplayFrame::UpdateTime, this);
    whiteTimer->Start(1000);
}

bool checkTimerRunning(wxTimer* timer) {
    return timer != nullptr && timer->IsRunning();
}

GameplayFrame::~GameplayFrame() {
    if (checkTimerRunning(whiteTimer)) {
        whiteTimer->Stop();
    }
    if (checkTimerRunning(blackTimer)) {
        blackTimer->Stop();
    }
    delete whiteTimer;
    delete blackTimer;
}

std::string GameplayFrame::secondsToString(int seconds) {
    if (seconds < 0) {
        return "Invalid input";
    }
    int minutes = seconds / 60;
    int remainingSecondsFinal = seconds % 60;
    // Construct the formatted string
    char buffer[100]; // Buffer to hold the formatted string
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, remainingSecondsFinal);
    return std::string(buffer);
}

void GameplayFrame::UpdateTime(wxTimerEvent& event) {
    wxTimer* timer = &(event.GetTimer());
    if (!timer)
        return; // Safety check

    // Get the ID of the timer that triggered the event
    int timerId = timer->GetId();
    // std::cout << "Id del timer di turno: " << timerId << " Id whiteTimer: " << whiteTimer->GetId() << " Id blackTimer: " << blackTimer->GetId() << std::endl;
    if (timerId == whiteTimer->GetId()) {
        whiteSeconds--;
        whiteTimerText->SetLabel(secondsToString(whiteSeconds));
    } else if (timerId == blackTimer->GetId()) {
        blackSeconds--;
        blackTimerText->SetLabel(secondsToString(blackSeconds));
    }
}

void GameplayFrame::StopUpdateTimer() {
    if (checkTimerRunning(whiteTimer)) {
        whiteTimer->Stop();
    }
    if (checkTimerRunning(blackTimer)) {
        blackTimer->Stop();
    }
}

void GameplayFrame::ChangeTimer() {
    if (checkTimerRunning(whiteTimer)) {
        whiteTimer->Stop();
        blackTimer->Start(1000);
    }
    else if (checkTimerRunning(blackTimer)) {
        blackTimer->Stop();
        whiteTimer->Start(1000);
    }
}


void GameplayFrame::StartGame() {
}

wxStaticBitmap* GameplayFrame::GetBoard() { return chessboard->GetBoard(); }
ChessboardView* GameplayFrame::GetChessboard() {return chessboard;}