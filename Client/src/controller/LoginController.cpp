#include "LoginController.hpp"

LoginController::LoginController(wxPanel *panel, RedisManager *red)
{
    this->panel = new LoginPanel(panel);
    this->backPanel = panel;
    this->red = red;
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
        return;
    }

    Richiesta richiesta;
    richiesta.codice = static_cast<int>(CodiceRichiesta::new_user);
    richiesta.input = {{"username", username}};
    json j = richiesta;

    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        panel->ShowError("Error contacting the server");
        return;
    }

    if (!red->SubscribeToChannel(username.c_str()))
    {
        panel->ShowError("Error contacting the server");
        return;
    }
    auto f = [this]()
    {
        std::string response = red->WaitResponse();
        std::cout << "Response: " << response << std::endl;
        if (!red->UnsubscribeFromChannel())
        {
            panel->ShowError("Error contacting the server");
            return;
        }
        panel->CallAfter([this]()
                         {
            backPanel->Show();
            panel->Hide(); });
    };

    std::thread t(f);
    t.detach();
}