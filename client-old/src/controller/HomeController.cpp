#include "HomeController.h"

// Constructor
HomeController::HomeController(/* args */)
{
    // istanzia frame con l'HomePage
    frame = new HomePage("Gioco degli scacchi",wxPoint(10, 20),wxSize(800,600));
//    std::string imgpath = "../../resources/img/";
//    const wxIcon icona = new wxIcon(wxString(imgpath+"chessboard.png"), wxBITMAP_TYPE_PNG);
//    frame->SetIcon(icona);
    frame->Show(true);

    wxInitAllImageHandlers();
    addButtonEvents();

    gamemodeController = new GamemodeController(frame->GetHomePanel());
    settingsController = new SettingsController(frame->GetHomePanel());
    profileController = new ProfileController(frame->GetHomePanel());
}

void HomeController::addButtonEvents() {
    frame->GetBtnPlay()->Bind(wxEVT_BUTTON, &HomeController::showGamemode, this);

    frame->GetBtnSettings()->Bind(wxEVT_BUTTON, &HomeController::showSettings, this);

    frame->GetBtnProfile()->Bind(wxEVT_BUTTON, &HomeController::showProfile, this);

    frame->GetBtnExit()->Bind(wxEVT_BUTTON, &HomeController::Exit, this);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("ViewGame");
    // }, ID_btnViewGame);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("Exit");
    // }, ID_btnClose);
}

void HomeController::showGamemode(wxCommandEvent& event){
    // GamemodeController* game = new GamemodeController(frame->GetHomePanel());
    auto configuration = settingsController->GetGameGraphicOptions();
    gamemodeController->SetGameGraphicOptions(configuration);
    gamemodeController->ShowPanel();
    frame->HidePanel();
}

void HomeController::showSettings(wxCommandEvent& event){
     // SettingsController* settings = new SettingsController(frame->GetHomePanel());
     settingsController->ShowPanel();
     frame->HidePanel();
}

void HomeController::showProfile(wxCommandEvent& event){
    // ProfileController* profile = new ProfileController(frame->GetHomePanel());
    profileController->ShowPanel();
    frame->HidePanel();
}

void HomeController::Exit(wxCommandEvent& event){

    exit(1);
}

HomeController::~HomeController()
{
}