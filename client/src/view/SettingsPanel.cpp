#include <wx-3.0/wx/wx.h>
#include "SettingsPanel.h"


SettingsPanel::SettingsPanel(wxWindow* parent): wxPanel(parent) {
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    std::string imgpath = "../resources/img/";

    //usiamo questa come cornice e gliela spostiamo intorno
    wxImage blackSquare = img::GetImageAndScale(imgpath + "black_square.png", 1.6);
    wxStaticBitmap* blackSquareBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(blackSquare));
    blackSquareBitmap->Move(90, 5);

    wxImage chessboardBrown = img::GetImageAndScale(imgpath + "chessboard-brown.png", 0.1);
    wxStaticBitmap* chessboardBrownBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboardBrown));
    chessboardBrownBitmap->Move(100, 10);

    wxImage chessboardBlue = img::GetImageAndScale(imgpath + "chessboard-blue.png", 0.1);
    wxStaticBitmap* chessboardBlueBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboardBlue));
    chessboardBlueBitmap->Move(200, 10);

    wxImage chessboardBlack = img::GetImageAndScale(imgpath + "chessboard-black.png", 0.1);
    wxStaticBitmap* chessboardBlackBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboardBlack));
    chessboardBlackBitmap->Move(300, 10);

    chessboardBrownBitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::BrownClick, this);
    chessboardBlueBitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::BlueClick, this);
    chessboardBlackBitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::BlackClick, this);

    wxImage pieces1 = img::GetImageAndScale(imgpath + "icons/black_horse.png", 1.8);
    wxStaticBitmap* pieces1Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces1));
    pieces1Bitmap->Move(100, 150);

    wxImage pieces2 = img::GetImageAndScale(imgpath + "icons/white_horse.png", 1.8);
    wxStaticBitmap* pieces2Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces2));
    pieces2Bitmap->Move(200, 150);

    wxImage pieces3 = img::GetImageAndScale(imgpath + "icons/black_horse.png", 1.8);
    wxStaticBitmap* pieces3Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces3));
    pieces3Bitmap->Move(300, 150);

    btnBack = new wxButton(this, wxID_ANY, "<-",wxPoint(100,500));
    btnSave = new wxButton(this, wxID_ANY, "Save",wxPoint(200,500));
}

void SettingsPanel::BrownClick(wxMouseEvent& event) {
    wxLogMessage("YOU CLICKED BROWN!!!");
}
void SettingsPanel::BlueClick(wxMouseEvent& event) {
    wxLogMessage("YOU CLICKED BLUE!!!");
}
void SettingsPanel::BlackClick(wxMouseEvent& event) {
    wxLogMessage("YOU CLICKED BLACK!!!");
}
wxButton* SettingsPanel::GetBtnBack() { return btnBack; }
wxButton* SettingsPanel::GetBtnSave() { return btnSave; }