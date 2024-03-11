#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <wx-3.0/wx/wx.h>
#include "../view/ProfilePanel.h"

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

