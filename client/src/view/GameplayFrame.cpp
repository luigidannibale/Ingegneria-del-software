
#include "GameplayFrame.h"
#include <thread>
#include <chrono>


GameplayFrame::GameplayFrame(bool isWhite, GameOptions* options): wxFrame(NULL, wxID_ANY, wxString("Gioca la partita vinci la fatica"), wxPoint(0,0), wxSize(0,0), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {
    float scal =  static_cast<float>(wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y )) / 1080; //optimized for 1080 width screen and scaled to be played on all screen
    chessX *= scal;
    chessY *= scal;
    SetSize(1440 * scal, 810* scal); //1440x810 is 3/4 of a 1920x1080 screen
    Center();
    Show();
    SetBackgroundColour(wxColour(118,150,86));

    wxImage boardImg = img::GetImageAndScale(IMGPATH + "chessboard-blue.png",scal);
    chessboard = new ChessboardView(new wxStaticBitmap(this, wxID_ANY, wxBitmap(boardImg)),"icons/", chessX, chessY, isWhite,scal);

    opponentText = new wxStaticText(this, wxID_ANY,wxString("Opponent name"));
    opponentText->Move(100*scal,20*scal);
    userText = new wxStaticText(this, wxID_ANY,wxString("Io"));
    userText->Move(100*scal,750*scal);


    const char* labelY[] = {"87654321","12345678"};
    const char* labelX[] =  {"ABCDEFGH","HGFEDCBA"};

    wxFont font(wxFontInfo(14).Family(wxFONTFAMILY_MODERN));
    wxStaticText* prova = new wxStaticText(this, wxID_ANY, wxString(""));
    wxSize fontSize = prova->GetFont().GetPixelSize();

    int index = isWhite?0:1;
    for (int i = 0; i < 8; i ++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString(labelY[index][i]));
        // label->SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessboard->GetBoard()->GetSize().GetWidth() + chessX+5, (chessY + i * GetChessboard()->GetCellDimension() + (GetChessboard()->GetCellDimension()/2 + fontSize.GetHeight()) / 2));
    }
    for (int i = 0; i < 8; i ++) {
        wxStaticText* label = new wxStaticText(this, wxID_ANY, wxString(labelX[index][i]));
        // label-SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessX+ i*GetChessboard()->GetCellDimension() + GetChessboard()->GetCellDimension()/2 , chessY-fontSize.GetHeight()-5);
    }


    movesPlayedList = new wxListView(this, wxID_ANY, wxPoint(800*scal, 200*scal), wxSize(300*scal, 200*scal), wxLC_REPORT);
    movesPlayedList->InsertColumn(0, "Num.", wxLIST_FORMAT_LEFT, 50*scal);
    movesPlayedList->InsertColumn(1, "Piece Moved", wxLIST_FORMAT_LEFT, 100*scal);
    movesPlayedList->InsertColumn(2, "Destination Square", wxLIST_FORMAT_LEFT, 150*scal);

    this->whiteSeconds = options->GetGameDurationInSeconds();
    this->blackSeconds = options->GetGameDurationInSeconds();
    this->increment = options->GetGameIncrement();
    whiteStatText = new wxStaticText(this, wxID_ANY,wxString("White timer"));
    whiteStatText->Move(1000*scal,60*scal);
    blackStatText = new wxStaticText(this, wxID_ANY,wxString("Black timer"));
    blackStatText->Move(1000*scal,460*scal);

    std::string time = secondsToString(options->GetGameDurationInSeconds());
    whiteTimerText = new wxStaticText(this, wxID_ANY, wxString(time));
    blackTimerText = new wxStaticText(this, wxID_ANY, wxString(time));
    whiteTimerText->Move(1000*scal, 100*scal);
    blackTimerText->Move(1000*scal, 500*scal);

    whiteTimer = new wxTimer(this, wxID_ANY);
    blackTimer = new wxTimer(this, wxID_ANY);
    
    Bind(wxEVT_TIMER, &GameplayFrame::UpdateTime, this);
    
    transparentPanel = new wxPanel(this, wxID_ANY, wxPoint(0,0), wxSize(1440*scal,810*scal));
    transparentPanel->SetBackgroundColour(wxColour(128, 120,120,120));
    loadingText = new wxTextCtrl(transparentPanel, wxID_ANY, wxString("Loading..."), wxDefaultPosition, transparentPanel->GetSize(), wxTE_CENTER | wxBORDER_NONE | wxTE_READONLY);  
    loadingText->SetForegroundColour(wxColour(0, 0, 0));
    loadingText->SetFont(wxFont(wxFontInfo(34*scal).Family(wxFONTFAMILY_MODERN)));
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    vSizer->AddStretchSpacer();
    vSizer->Add(loadingText, wxSizerFlags().Expand());
    vSizer->AddStretchSpacer();
    transparentPanel->SetSizer(vSizer);
    transparentPanel->Show();
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

void GameplayFrame::ShowTransparentPanel() {
    transparentPanel->Show();
}

void GameplayFrame::HideTransparentPanel() {
    transparentPanel->Hide();
}

void GameplayFrame::UpdateTransparentPanel(std::string text) {
    loadingText->SetValue(text);
}

std::string GameplayFrame::secondsToString(int seconds) {
    if (seconds < 0) {
        return "Invalid input";
    }
    int minutes = seconds / 60;
    seconds-= minutes*60;
    int hours = minutes / 60;
    minutes-= hours*60;
    //int remainingSecondsFinal = seconds % 60;
    // Construct the formatted string
    char buffer[100]; // Buffer to hold the formatted string
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",hours, minutes, seconds);
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
        whiteSeconds+=increment;
        whiteTimerText->SetLabel(secondsToString(whiteSeconds));
        blackTimer->Start(1000);
    }
    else if (checkTimerRunning(blackTimer)) {
        blackTimer->Stop();
        blackSeconds+=increment;
        blackTimerText->SetLabel(secondsToString(blackSeconds));
        whiteTimer->Start(1000);
    }
}

void GameplayFrame::StartTimer() {
    whiteTimer->Start(1000);
}

void GameplayFrame::StartGame() {
}

wxStaticBitmap* GameplayFrame::GetBoard() { return chessboard->GetBoard(); }
ChessboardView* GameplayFrame::GetChessboard() {return chessboard;}