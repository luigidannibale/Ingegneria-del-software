#include <stdio.h>
// #include <wx/wx.h>
#include "../view/ProfilePanel.h"

class ProfileController
{
    public:
        ProfileController(wxPanel*);
        ~ProfileController();

    private:
        ProfilePanel* panel;
        wxPanel* backPanel;

        void addButtonEvents();

        void BackPanel(wxCommandEvent&);
};

// Constructor
ProfileController::ProfileController(wxPanel* parent)
{
    // istanzia frame con l'HomePage
   panel = new ProfilePanel(parent->GetParent());
   panel->Show(true);
   backPanel = parent; 
//    prova = new LoginPanel(parent);
//    prova->Show(true);
    // prova1 = new ProfilePanel(parent);
    // prova1->Show(true);

    addButtonEvents();
}

void ProfileController::addButtonEvents() {
    //panel->GetBtnBack()->Bind(wxEVT_BUTTON, &ProfileController::BackPanel, this);
}

void ProfileController::BackPanel(wxCommandEvent& event) {
    backPanel->Show();
    panel->Hide();
}

ProfileController::~ProfileController()
{
}

