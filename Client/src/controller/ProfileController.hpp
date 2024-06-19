#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include "../lib.hpp"
#include "../view/ProfilePanel.hpp"

class ProfileController
{
    public:
        ProfileController(wxPanel*);
        ~ProfileController();

        void ShowPanel();

    private:
        ProfilePanel* panel;
        wxPanel* backPanel;
        void addButtonEvents();
        void BackPanel(wxCommandEvent&);
        void SaveUser(wxCommandEvent&);
};



#endif // PROFILECONTROLLER_

