#include <wx/wx.h>

enum {
    ID_Qualcosa = 1,
    ID_btnBack
};

class MatchmakingPanel : public wxPanel {
public:
    MatchmakingFrame(wxWindow* parent, wxPanel* back);
private: 
    void btnBack(wxCommandEvent& event);

    wxPanel *back;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(GameMode, wxPanel)
    EVT_BUTTON(ID_btnBack, MatchmakingPanel::btnBack)
wxEND_EVENT_TABLE()

MatchmakingPanel::MatchmakingPanel(wxWindow* parent): wxPanel(parent) {
    // Add controls or customizations for the PlayPanel
    wxStaticText* playText = new wxStaticText(this, wxID_ANY, "This is the PlayPanel", wxPoint(10, 10));

    // Add a button inside PlayPanel
    wxButton* btnBack = new wxButton(this, ID_btnBack, "<--", wxPoint(10, 50));

    Bind(wxEVT_BUTTON, &MatchmakingPanel::btnBack, this, ID_btnBack);
}

void GameMode::btnBack(wxCommandEvent& event) {    
    backPanel->Show();
    Hide();
    Destroy();
}

