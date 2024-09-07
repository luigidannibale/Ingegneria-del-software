#include "HomeController.hpp"

// Constructor
HomeController::HomeController(/* args */)
{

    frame = new HomePage("Gioco degli scacchi", wxPoint(10, 20), wxSize(800, 600));
    //    const wxIcon icona = new wxIcon(wxString(imgpath+"chessboard.png"), wxBITMAP_TYPE_PNG);
    //    frame->SetIcon(icona);
    frame->Show(true);

    wxInitAllImageHandlers();
    addButtonEvents();

    red = new RedisManager();
    user = User("username", "nome", "cognome", 0);
    configuration = GameGraphicOptions(BoardStyle::brown, PiecesStyle::neo);

    gamemodeController = new GamemodeController(frame->GetHomePanel(), red, user);
    settingsController = new SettingsController(frame->GetHomePanel(), red, configuration);
    profileController = new ProfileController(frame->GetHomePanel(), red, user);
    viewGameController = new ViewGameController(frame->GetHomePanel(), red);
    loginController = new LoginController(frame->GetHomePanel(), red, user, configuration);

    loginController->ShowPanel();
}

void HomeController::addButtonEvents()
{
    frame->GetBtnPlay()->Bind(wxEVT_BUTTON, &HomeController::showGamemode, this);

    frame->GetBtnSettings()->Bind(wxEVT_BUTTON, &HomeController::showSettings, this);

    frame->GetBtnProfile()->Bind(wxEVT_BUTTON, &HomeController::showProfile, this);

    frame->GetBtnViewGame()->Bind(wxEVT_BUTTON, &HomeController::showViewGame, this);

    frame->GetBtnExit()->Bind(wxEVT_BUTTON, &HomeController::Exit, this);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("ViewGame");
    // }, ID_btnViewGame);

    // frame->Bind(wxEVT_BUTTON, [=](wxCommandEvent&){
    //     wxLogMessage("Exit");
    // }, ID_btnClose);
}

void HomeController::showGamemode(wxCommandEvent &event)
{
    // GamemodeController* game = new GamemodeController(frame->GetHomePanel());
    auto configuration = settingsController->GetGameGraphicOptions();
    gamemodeController->SetGameGraphicOptions(configuration);
    gamemodeController->ShowPanel();
    frame->HidePanel();
}

void HomeController::showSettings(wxCommandEvent &event)
{
    // SettingsController* settings = new SettingsController(frame->GetHomePanel());
    settingsController->SetGameGraphicOptions(&configuration);
    settingsController->SetUsername(user.Username());
    settingsController->ShowPanel();
    frame->HidePanel();
}

void HomeController::showProfile(wxCommandEvent &event)
{
    // ProfileController* profile = new ProfileController(frame->GetHomePanel());
    std::cout << user.Username() << std::endl;
    profileController->UpdateProfile(user);
    profileController->ShowPanel();
    frame->HidePanel();
}

void HomeController::showViewGame(wxCommandEvent &event)
{
    viewGameController->SetGameGraphicOptions(&configuration);
    viewGameController->ShowPanel(user.Username());
    frame->HidePanel();
}

void HomeController::Exit(wxCommandEvent &event)
{
    exit(1);
}

HomeController::~HomeController()
{
}