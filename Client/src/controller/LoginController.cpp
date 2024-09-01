#include "LoginController.hpp"

LoginController::LoginController(wxPanel *panel, RedisManager *red, User &user, GameGraphicOptions &configuration)
{
    this->panel = new LoginPanel(panel);
    this->backPanel = panel;
    this->red = red;
    this->user = &user;
    this->configuration = &configuration;
    addButtonEvents();
}

LoginController::~LoginController()
{
    delete panel;
}

void LoginController::addButtonEvents()
{
    // panel->getLoginButton()->Bind(wxEVT_BUTTON, &LoginController::onLoginButtonClick, this);
    panel->getLoginButton()->Bind(wxEVT_BUTTON, &LoginController::Login, this);
}

void LoginController::ShowPanel()
{
    panel->Show();
}

void LoginController::Login(wxCommandEvent &event)
{
    wxTextCtrl *usernameCtrl = panel->getUsername();
    std::string username = usernameCtrl->GetValue().ToStdString();

    if (username.empty())
    {
        panel->ShowError("Username cannot be empty");
        return;
    }

    if (!red->Connect())
    {
        panel->ShowError("Can't connect to server");
        red->Disconnect();
        return;
    }

    if (!red->CheckChannel(red->SERVER_CHANNEL))
    {
        panel->ShowError("Error contacting the server");
        red->Disconnect();
        return;
    }

    Messaggio richiesta;
    richiesta.codice = static_cast<int>(CodiceRichiesta::new_user);
    richiesta.input = {{"username", username}};
    json j = richiesta;

    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        panel->ShowError("Error contacting the server");
        red->Disconnect();
        return;
    }

    if (!red->SubscribeToChannel(username.c_str()))
    {
        panel->ShowError("Error contacting the server");
        red->Disconnect();
        return;
    }
    auto f = [this]()
    {
        std::string response = red->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (nlohmann::json::parse_error &e)
        {
            wxString errore = wxString::FromUTF8(("Cannot parse message: " + response).c_str());
            panel->ShowError(errore);
            red->Disconnect();
            return;
        }
        if (!red->UnsubscribeFromChannel())
        {
            panel->ShowError("Error contacting the server");
            red->Disconnect();
            return;
        }
        if (risposta.codice == static_cast<int>(CodiceRisposta::server_error))
        {
            panel->ShowError("Error contacting the server");
            red->Disconnect();
            return;
        }
        if (risposta.codice != static_cast<int>(CodiceRisposta::user_created))
        {
            panel->ShowError("Unexpected response from server");
            red->Disconnect();
            return;
        }
        std::string username = risposta.input["username"];
        std::string nome = risposta.input["nome"];
        std::string cognome = risposta.input["cognome"];
        int puntiElo = risposta.input["puntiElo"];

        BoardStyle boardStyle = risposta.input.at("c_st").get<BoardStyle>();
        PiecesStyle piecesStyle = risposta.input.at("p_st").get<PiecesStyle>();

        user->SetUsername(username);
        user->SetName(nome);
        user->SetSurname(cognome);
        user->SetEloPoints(puntiElo);

        configuration->SetBoardStyle(boardStyle);
        configuration->SetPiecesStyle(piecesStyle);

        panel->CallAfter([this]()
                         {
            backPanel->Show();
            panel->Hide(); });
    };

    std::thread t(f);
    t.detach();
}