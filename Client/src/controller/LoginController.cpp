#include "LoginController.hpp"

LoginController::LoginController(wxPanel *panel, RedisManager* red)
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
    wxTextCtrl* usernameCtrl = panel->getUsername();
    std::string username = usernameCtrl->GetValue().ToStdString();

    if(username.empty())
    {
        panel->ShowError("Username cannot be empty");
        return;
    }

    if (!red->Connect()) {
        panel->ShowError("Can't connect to server");
        return;
    }

    if (!red->PublishToChannel(red->SERVER_CHANNEL, username.c_str())) {
        panel->ShowError("Error contacting the server");
        return;
    }

    backPanel->Show();
    panel->Hide();
}