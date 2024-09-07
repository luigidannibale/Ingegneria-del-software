#ifndef VIEWGAMEPANEL_H
#define VIEWGAMEPANEL_H

#include "../lib.hpp"
#include <wx/sizer.h>
#include <wx/grid.h>
#include <iostream>

class ViewGamePanel : public wxPanel
{
public:
    ViewGamePanel(wxWindow *parent);
    ~ViewGamePanel();

    wxGrid *GetTable();
    wxButton *GetBackButton();

    void SetTableHeight();

private:
    wxGrid *table;
    wxButton *backButton;
};

#endif // VIEWGAMEPANEL_H