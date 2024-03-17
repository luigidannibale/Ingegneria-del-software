#include "GameMode.h"

GameMode::GameMode(wxWindow* parent): wxPanel(parent) {
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    btnMultiplayer = new wxButton(this, wxID_ANY, "Play online",wxPoint(10,50));
    btnComputer = new wxButton(this, wxID_ANY, "Play against computer",wxPoint(10,100));
    btnBack = new wxButton(this, wxID_ANY, "Back",wxPoint(10,200));

    multiplayerPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);
    initPanel(multiplayerPanel);

    // Cadenza di gioco
    wxArrayString gameTime;
    gameTime.Add("3|2");
    gameTime.Add("10|1");
    gameTime.Add("15|10");
    gameTime.Add("Infinite");
    multiplayerTimeBox = new wxRadioBox(multiplayerPanel, wxID_ANY, "Choose the time duration \n(the number after the | is the increments in seconds)", wxDefaultPosition, wxDefaultSize, gameTime, 1, wxRA_SPECIFY_COLS | wxNO_BORDER);
    multiplayerTimeBox->SetPosition(wxPoint(20,20));
    multiplayerTimeBox->SetSelection(1);

    btnStartMultiplayer = new wxButton(multiplayerPanel, wxID_ANY, "Start", wxPoint(295, 450));
    // btnStartMultiplayer->Bind(wxEVT_BUTTON, &GameMode::StartMultiplayerGame, this);

    computerPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);
    initPanel(computerPanel);

    // Cadenza di gioco
    computerTimeBox = new wxRadioBox(computerPanel, wxID_ANY, "Choose the time duration \n(the number after the | is the increments in seconds)", wxDefaultPosition, wxDefaultSize, gameTime, 2, wxRA_SPECIFY_COLS | wxNO_BORDER);
    computerTimeBox->SetPosition(wxPoint(20,20));
    computerTimeBox->SetSelection(1);

    btnStartComputer = new wxButton(computerPanel, wxID_ANY, "Start", wxPoint(295, 450));
    // btnStartComputer->Bind(wxEVT_BUTTON, &GameMode::StartComputerGame, this);

    // Difficoltà
    wxArrayString computerElo;
    computerElo.Add("Beginner");
    computerElo.Add("Intermediate");
    computerElo.Add("Advanced");
    computerElo.Add("Master");
    difficultyBox = new wxRadioBox(computerPanel, wxID_ANY, "Choose computer elo", wxDefaultPosition, wxDefaultSize, computerElo, 1, wxRA_SPECIFY_COLS | wxNO_BORDER);
    difficultyBox->SetPosition(wxPoint(20,120));
    difficultyBox->SetSelection(1);

    // Side
    wxArrayString startSide;
    startSide.Add("Casual");
    startSide.Add("White");
    startSide.Add("Black");
    startSideBox = new wxRadioBox(computerPanel, wxID_ANY, "Choose your pieces", wxDefaultPosition, wxDefaultSize, startSide, 1, wxRA_SPECIFY_COLS | wxNO_BORDER);
    startSideBox->SetPosition(wxPoint(20,250));


    multiplayerPanel->Hide();
    computerPanel->Hide();
}

GameTime* GameMode::GetGameTime() {
    int duration, increment;
    switch (selectedDuration) {
        case 0:
            duration = 3 * 60;
            increment = 2;
            break;
        case 1:
            duration = 10 * 60;
            increment = 0;
            break;
        case 2:
            duration = 15 * 60;
            increment = 10;
            break;
        case 3:
            duration = 180*60;
            increment = 20;
            break;
    };

    return new GameTime(duration, increment);
}
GameOptions* GameMode::GetGameOptions() {
    bool againstHuman = multiplayerPanel->IsShown();

    if (againstHuman) selectedDuration = multiplayerTimeBox->GetSelection();
    else selectedDuration = computerTimeBox->GetSelection();

    selectedDifficulty = difficultyBox->GetSelection();
    selectedSide = startSideBox->GetSelection();

    return new GameOptions(againstHuman, GetGameTime(),
                                        static_cast<ComputerElo>(selectedDifficulty),
                                        static_cast<StartSide>(selectedSide));
}


void GameMode::initPanel(wxPanel* panel){
    panel->SetBackgroundColour(wxColour(118,150,86));
    panel->SetSize(400,500);
    panel->SetPosition(wxPoint(350,50));
}


wxButton* GameMode::GetBtnMultiplayer()     { return btnMultiplayer; }
wxButton* GameMode::GetBtnComputer()        { return btnComputer; }
wxButton* GameMode::GetBtnBack()            { return btnBack; }
wxButton* GameMode::GetBtnStartMultiplayer(){ return btnStartMultiplayer; }
wxButton* GameMode::GetBtnStartComputer()   { return btnStartComputer; }
wxPanel* GameMode::GetMultiplayerPanel()    { return multiplayerPanel; }
wxPanel* GameMode::GetComputerPanel()       { return computerPanel; }