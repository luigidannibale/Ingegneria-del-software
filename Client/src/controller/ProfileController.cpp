#include "ProfileController.hpp"

ProfileController::ProfileController(wxPanel *parent, RedisManager *red, User &user)
{
    this->user = &user;
    this->red = red;
    // User *user = new User("gigi", "Luigi", "D'annibale", 1300);
    panel = new ProfilePanel(parent->GetParent(), &user);
    panel->Hide();
    backPanel = parent;

    addButtonEvents();
}

void ProfileController::addButtonEvents()
{
    panel->GetBtnBack()->Bind(wxEVT_BUTTON, &ProfileController::BackPanel, this);
    panel->GetBtnSave()->Bind(wxEVT_BUTTON, &ProfileController::SaveUser, this);
    panel->GetBtnUpdate()->Bind(wxEVT_BUTTON, &ProfileController::ShowUpdate, this);
    panel->GetBtnCancel()->Bind(wxEVT_BUTTON, &ProfileController::ShowDefault, this);
}

void ProfileController::UpdateProfile(User &user)
{
    panel->UpdateUserDatas(&user);
}

void ProfileController::ShowDefault(wxCommandEvent &event)
{
    panel->ShowDefault();
}

void ProfileController::ShowUpdate(wxCommandEvent &event)
{
    panel->ShowUpdate();
}

void ProfileController::ShowPanel()
{
    panel->Show();
}

void ProfileController::BackPanel(wxCommandEvent &event)
{
    backPanel->Show();
    panel->Hide();
}

void ProfileController::SaveUser(wxCommandEvent &event)
{
    // User oldUser = *user; // Copy of the user

    std::string username = panel->GetTxtUsername()->GetValue().ToStdString();
    std::string name = panel->GetTxtName()->GetValue().ToStdString();
    std::string surname = panel->GetTxtSurname()->GetValue().ToStdString();

    Messaggio richiesta;
    richiesta.codice = static_cast<int>(CodiceRichiesta::update_user);
    richiesta.input["username"] = user->Username();
    richiesta.input["new_username"] = username.empty() ? user->Username() : username;
    richiesta.input["nome"] = name.empty() ? user->Name() : name;
    richiesta.input["cognome"] = surname.empty() ? user->Surname() : surname;
    richiesta.input["elo"] = user->EloPoints();
    json j = richiesta;

    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        wxLogMessage("Error while updating user");
        return;
    }

    if (!red->SubscribeToChannel(user->Username().c_str()))
    {
        wxLogMessage("Error while updating user");
        return;
    }

    auto f = [this, username, name, surname]()
    {
        std::string response = red->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (const std::exception &e)
        {
            wxLogMessage("Error while updating user");
            return;
        }

        if (risposta.codice == static_cast<int>(CodiceRisposta::ok))
        {
            user->SetUsername(username);
            user->SetName(name);
            user->SetSurname(surname);

            panel->CallAfter([this]()
                             {
                panel->UpdateUserDatas(user);
                panel->ShowDefault(); });
        }
        else if (risposta.codice == static_cast<int>(CodiceRisposta::bad_request))
        {
            wxLogMessage("USername already taken");
        }
        else if (risposta.codice == static_cast<int>(CodiceRisposta::server_error))
        {
            wxLogMessage("Error while updating user");
        }

        if (!red->UnsubscribeFromChannel(user->Username().c_str()))
        {
            wxLogMessage("Error contacting the server");
            return;
        }
    };

    std::thread t(f);
    t.detach();
}

ProfileController::~ProfileController()
{
}