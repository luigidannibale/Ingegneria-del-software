#include "ViewGameController.hpp"

ViewGameController::ViewGameController(wxPanel *backPanel, RedisManager *red)
{
    panel = new ViewGamePanel(backPanel);
    this->red = red;
    this->backPanel = backPanel;

    panel->Hide();

    addButtonEvents();
}

ViewGameController::~ViewGameController()
{
}

void ViewGameController::addButtonEvents()
{
    panel->GetTable()->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &ViewGameController::OnCellClick, this);
    panel->GetBackButton()->Bind(wxEVT_BUTTON, &ViewGameController::Back, this);
}

void ViewGameController::ShowPanel(std::string username)
{
    this->username = username;
    Messaggio messaggio;
    messaggio.codice = static_cast<int>(CodiceRichiesta::list_games);
    messaggio.input["username"] = username;
    json j = messaggio;

    panel->Show();
    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    if (!red->SubscribeToChannel(username.c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    auto f = [this]()
    {
        std::string response = this->red->WaitResponse();
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (const std::exception &e)
        {
            wxLogMessage("Error parsing server response");
            return;
        }

        if (risposta.codice != static_cast<int>(CodiceRisposta::game_found))
        {
            wxLogMessage("Error contacting the server");
            return;
        }

        std::vector<Game> games;
        try
        {
            games = risposta.input.get<std::vector<Game>>();
        }
        catch (const std::exception &e)
        {
            wxLogMessage("Error parsing server response");
            return;
        }

        panel->CallAfter([games, this]()
                         {         
        wxGrid *table = panel->GetTable();
        table->ClearGrid();

        for (int i = 0; i < games.size(); i++)
        {
            try {
                table->AppendRows(1);
                table->SetCellValue(i, 0, std::to_string(games[i].ID));
                table->SetCellValue(i, 1, games[i].u_id_w);
                table->SetCellValue(i, 2, games[i].u_id_b);
                std::string esito = json(games[i].esito);
                table->SetCellValue(i, 3, esito);
                std::string motivo = json(games[i].motivo);
                table->SetCellValue(i, 4, motivo);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error setting cell value " << e.what() <<std::endl;
                if (table->GetNumberRows() > 0)
                    table->DeleteRows(table->GetNumberRows() - 1);
                return;
            }
        }

        panel->SetTableHeight(); });
    };

    std::thread t(f);
    t.detach();
}

void ViewGameController::OnCellClick(wxGridEvent &event)
{
    int row = event.GetRow();
    std::string cellValue = panel->GetTable()->GetCellValue(row, 0).ToStdString();
    int value = std::stoi(cellValue);

    Messaggio messaggio;
    messaggio.codice = static_cast<int>(CodiceRichiesta::search_game);
    messaggio.input["game_id"] = value;
    json j = messaggio;

    if (!red->PublishToChannel(red->SERVER_CHANNEL, j.dump().c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    std::string channel = "game" + std::to_string(value);
    if (!red->SubscribeToChannel(channel.c_str()))
    {
        wxLogMessage("Error contacting the server");
        return;
    }

    auto f = [this]()
    {
        std::string response = red->WaitResponse(true);
        Messaggio risposta;
        try
        {
            risposta = json::parse(response);
        }
        catch (const std::exception &e)
        {
            wxLogMessage("Error parsing server response");
            return;
        }

        if (risposta.codice != static_cast<int>(CodiceRisposta::game_found))
        {
            wxLogMessage("Error contacting the server");
            return;
        }

        Game game = risposta.input.get<Game>();

        panel->CallAfter([game, this]()
                         {
            panel->GetParent()->Hide();
            bool isWhite = game.u_id_w.compare(username) == 0;
            GameplayController *gameplayController = new GameplayController(backPanel, gameGraphicOptions, game, isWhite); });
    };

    std::thread t(f);
    t.detach();
}

void ViewGameController::Back(wxCommandEvent &event)
{
    panel->Hide();
    backPanel->Show();
}

void ViewGameController::SetGameGraphicOptions(GameGraphicOptions *options)
{
    gameGraphicOptions = options;
}
