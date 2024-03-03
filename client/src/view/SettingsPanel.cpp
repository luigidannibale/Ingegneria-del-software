#include <wx-3.0/wx/wx.h>
#include "SettingsPanel.h"

SettingsPanel::SettingsPanel(wxWindow* parent): wxPanel(parent) {
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

//    std::string imgpath = "../../resources/img/";
//    wxIcon icona(wxString(imgpath+"chessboard.png"));
//    //GetParent()->SetIcon(icona);
//    wxStaticBitmap* staticBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(icona));
//    staticBitmap->Move(10, 10); // Adjust the position as needed

    btnBack = new wxButton(this, wxID_ANY, "<-",wxPoint(10,200));
    btnSave = new wxButton(this, wxID_ANY, "Save",wxPoint(100,200));
}

wxButton* SettingsPanel::GetBtnBack() { return btnBack; }
wxButton* SettingsPanel::GetBtnSave() { return btnSave; }