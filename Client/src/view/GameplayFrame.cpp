
#include "GameplayFrame.hpp"

GameplayFrame::GameplayFrame(bool isWhite, GameOptions *options, GameGraphicOptions *graphicOptions, std::string user_id, std::string opponent_id) : wxFrame(NULL, wxID_ANY, wxString("Gioco degli scacchi"), wxPoint(0, 0), wxSize(0, 0), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    float scal = static_cast<float>(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)) / 1080; // optimized for 1080 width screen and scaled to be played on all screen
    chessX *= scal;
    chessY *= scal;
    SetSize(1440 * scal, 810 * scal); // 1440x810 is 3/4 of a 1920x1080 screen
    Center();
    Show();
    SetBackgroundColour(wxColour(118, 150, 86));

    wxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel *leftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    leftPanel->SetBackgroundColour(wxColour(118, 150, 86));
    wxPanel *rightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightPanel->SetBackgroundColour(wxColour(118, 150, 86));
    wxImage boardImg;
    std::string board;
    switch (graphicOptions->GetBoardStyle())
    {
    case BoardStyle::blue:
        board = "chessboard-blue.png";
        break;
    case BoardStyle::brown:
        board = "chessboard-brown.png";
        break;
    case BoardStyle::black:
        board = "chessboard-black.png";
        break;
    }

    std::string iconset;
    switch (graphicOptions->GetPiecesStyle())
    {
    case PiecesStyle::neo:
        // case PiecesStyle::neo2 :
        iconset = "icons_neo/";
        break;
    case PiecesStyle::pixel:
        iconset = "icons_pixel/";
        break;
    }

    boardImg = img::GetImageAndScale(board, scal);
    chessboard = new ChessboardView(new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(boardImg)), iconset, chessX, chessY, isWhite, scal);
    opponentText = new wxStaticText(rightPanel, wxID_ANY, opponent_id);
    opponentText->Move(100 * scal, 100 * scal);
    userText = new wxStaticText(rightPanel, wxID_ANY, user_id);
    userText->Move(100 * scal, 650 * scal);

    const char *labelY[] = {"87654321", "12345678"};
    const char *labelX[] = {"ABCDEFGH", "HGFEDCBA"};

    wxFont font(wxFontInfo(14).Family(wxFONTFAMILY_MODERN));
    wxStaticText *prova = new wxStaticText(leftPanel, wxID_ANY, wxString(""));
    wxSize fontSize = prova->GetFont().GetPixelSize();
    int index = isWhite ? 0 : 1;
    for (int i = 0; i < 8; i++)
    {
        wxStaticText *label = new wxStaticText(leftPanel, wxID_ANY, wxString(labelY[index][i]));
        // label->SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessboard->GetBoard()->GetSize().GetWidth() + chessX + 5, (chessY + i * GetChessboard()->GetCellDimension() + (GetChessboard()->GetCellDimension() / 2 + fontSize.GetHeight()) / 2));
    }
    for (int i = 0; i < 8; i++)
    {
        wxStaticText *label = new wxStaticText(leftPanel, wxID_ANY, wxString(labelX[index][i]));
        // label-SetFont(font);
        label->SetForegroundColour(wxColour(0, 0, 0)); // Black color
        label->Move(chessX + i * GetChessboard()->GetCellDimension() + GetChessboard()->GetCellDimension() / 2, chessY - fontSize.GetHeight() - 5);
    }

    movesPlayedList = new wxListView(rightPanel, wxID_ANY, wxPoint(80 * scal, 250 * scal), wxSize(300 * scal, 200 * scal), wxLC_REPORT);
    movesPlayedList->InsertColumn(0, "Num.", wxLIST_FORMAT_LEFT, 50 * scal);
    movesPlayedList->InsertColumn(1, "White Moves", wxLIST_FORMAT_LEFT, 100 * scal);
    movesPlayedList->InsertColumn(2, "Black Moves", wxLIST_FORMAT_LEFT, 150 * scal);

    btnNextMove = new wxButton(rightPanel, wxID_ANY, wxString("Next Move"), wxPoint(250 * scal, 500 * scal), wxSize(100 * scal, 50 * scal));
    btnPrevMove = new wxButton(rightPanel, wxID_ANY, wxString("Prev Move"), wxPoint(100 * scal, 500 * scal), wxSize(100 * scal, 50 * scal));
    btnNextMove->Hide();
    btnPrevMove->Hide();

    if (options != nullptr)
    {
        this->whiteSeconds = options->GetGameDurationInSeconds();
        this->blackSeconds = options->GetGameDurationInSeconds();
        this->increment = options->GetGameIncrement();

        std::string time = secondsToString(options->GetGameDurationInSeconds());
        whiteTimerText = new wxStaticText(rightPanel, wxID_ANY, wxString(time));
        blackTimerText = new wxStaticText(rightPanel, wxID_ANY, wxString(time));
        int whiteY = isWhite ? 600 : 200;
        int blackY = isWhite ? 200 : 600;
        whiteTimerText->Move(100 * scal, whiteY * scal);
        blackTimerText->Move(100 * scal, blackY * scal);
    }

    whiteTimer = new wxTimer(this, wxID_ANY);
    blackTimer = new wxTimer(this, wxID_ANY);

    mainSizer->Add(leftPanel, wxSizerFlags(1).Expand().Border(wxBOTTOM, 10));
    mainSizer->Add(rightPanel, wxSizerFlags(1).Expand());

    SetSizerAndFit(mainSizer);
    Layout();

    transparentPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), this->GetSize());
    transparentPanel->SetBackgroundColour(wxColour(128, 120, 120, 120));
    loadingText = new wxTextCtrl(transparentPanel, wxID_ANY, wxString("Loading..."), wxDefaultPosition, transparentPanel->GetSize(), wxTE_CENTER | wxBORDER_NONE | wxTE_READONLY);
    loadingText->SetForegroundColour(wxColour(0, 0, 0));
    loadingText->SetFont(wxFont(wxFontInfo(34 * scal).Family(wxFONTFAMILY_MODERN)));
    wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
    vSizer->AddStretchSpacer();
    vSizer->Add(loadingText, wxSizerFlags().Expand());
    vSizer->AddStretchSpacer();
    transparentPanel->SetSizer(vSizer);
    transparentPanel->Show();
    transparentPanel->SetTransparent(120);
}

bool checkTimerRunning(wxTimer *timer)
{
    return timer != nullptr && timer->IsRunning();
}

GameplayFrame::~GameplayFrame()
{
    if (checkTimerRunning(whiteTimer))
    {
        whiteTimer->Stop();
    }
    if (checkTimerRunning(blackTimer))
    {
        blackTimer->Stop();
    }
    delete whiteTimer;
    delete blackTimer;
}

void GameplayFrame::ShowTransparentPanel()
{
    transparentPanel->Show();
}

void GameplayFrame::HideTransparentPanel()
{
    transparentPanel->Hide();
}

void GameplayFrame::UpdateTransparentPanel(std::string text)
{
    loadingText->SetValue(text);
}

void GameplayFrame::AddMoveToList(std::string move)
{
    long index = movesPlayedList->GetItemCount();
    if (whiteMove)
        movesPlayedList->InsertItem(index, std::to_string(index + 1));
    else
        index--;
    movesPlayedList->SetItem(index, whiteMove ? 1 : 2, move);
    whiteMove = !whiteMove;
    movesPlayedList->EnsureVisible(index);
}

void GameplayFrame::RemoveLastMoveFromList()
{
    long index = movesPlayedList->GetItemCount() - 1;
    if (!whiteMove)
    {
        movesPlayedList->DeleteItem(index);
        index--;
    }
    else
        movesPlayedList->SetItem(index, 2, "");
    whiteMove = !whiteMove;
    if (index >= 0)
        movesPlayedList->EnsureVisible(index);
}

std::string GameplayFrame::secondsToString(int seconds)
{
    if (seconds < 0)
    {
        return "Invalid input";
    }
    int minutes = seconds / 60;
    seconds -= minutes * 60;
    int hours = minutes / 60;
    minutes -= hours * 60;
    // int remainingSecondsFinal = seconds % 60;
    //  Construct the formatted string
    char buffer[100]; // Buffer to hold the formatted string
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return std::string(buffer);
}

// returns 1 if white timer is over, 2 if black timer is over, 0 otherwise
int GameplayFrame::UpdateTime(wxTimerEvent &event)
{
    wxTimer *timer = &(event.GetTimer());
    if (!timer)
        return 0; // Safety check

    // Get the ID of the timer that triggered the event
    int timerId = timer->GetId();
    // std::cout << "Id del timer di turno: " << timerId << " Id whiteTimer: " << whiteTimer->GetId() << " Id blackTimer: " << blackTimer->GetId() << std::endl;
    if (timerId == whiteTimer->GetId())
    {
        whiteSeconds--;
        whiteTimerText->SetLabel(secondsToString(whiteSeconds));
        if (whiteSeconds == 0)
        {
            whiteTimer->Stop();
            return 1;
        }
    }
    else if (timerId == blackTimer->GetId())
    {
        blackSeconds--;
        blackTimerText->SetLabel(secondsToString(blackSeconds));
        if (blackSeconds == 0)
        {
            blackTimer->Stop();
            return 2;
        }
    }
    return 0;
}

void GameplayFrame::StopUpdateTimer()
{
    if (checkTimerRunning(whiteTimer))
    {
        whiteTimer->Stop();
    }
    if (checkTimerRunning(blackTimer))
    {
        blackTimer->Stop();
    }
}

void GameplayFrame::ChangeTimer()
{
    if (checkTimerRunning(whiteTimer))
    {
        whiteTimer->Stop();
        whiteSeconds += increment;
        whiteTimerText->SetLabel(secondsToString(whiteSeconds));
        if (blackSeconds > 0)
            blackTimer->Start(1000);
    }
    else if (checkTimerRunning(blackTimer))
    {
        blackTimer->Stop();
        blackSeconds += increment;
        blackTimerText->SetLabel(secondsToString(blackSeconds));
        if (whiteSeconds > 0)
            whiteTimer->Start(1000);
    }
}

void GameplayFrame::StartTimer()
{
    whiteTimer->Start(1000);
}

wxStaticBitmap *GameplayFrame::GetBoard() { return chessboard->GetBoard(); }
ChessboardView *GameplayFrame::GetChessboard() { return chessboard; }
wxButton *GameplayFrame::GetNextMoveButton() { return btnNextMove; }
wxButton *GameplayFrame::GetPrevMoveButton() { return btnPrevMove; }