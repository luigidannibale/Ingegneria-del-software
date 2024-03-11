#include <wx-3.0/wx/wx.h>
#include "SettingsPanel.h"


SettingsPanel::SettingsPanel(wxWindow* parent): wxPanel(parent) {
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));
    
    chessboard1 = img::GetImageAndScale(IMGPATH + "chessboard-brown.png", 0.1);
    chessboard1Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboard1));
    chessboard1Bitmap->Move(100, 10);

    chessboard2 = img::GetImageAndScale(IMGPATH + "chessboard-blue.png", 0.1);
    chessboard2Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboard2));
    chessboard2Bitmap->Move(200, 10);

    chessboard3 = img::GetImageAndScale(IMGPATH + "chessboard-black.png", 0.1);
    chessboard3Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(chessboard3));
    chessboard3Bitmap->Move(300, 10);

    chessboard1Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard1Click, this);
    chessboard2Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard2Click, this);
    chessboard3Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard3Click, this);

    pieces1 = img::GetImageAndScale(IMGPATH + "icons/black_horse.png", 1.8);
    pieces1Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces1));
    pieces1Bitmap->Move(100, 150);

    pieces2 = img::GetImageAndScale(IMGPATH + "icons/white_horse.png", 1.8);
    pieces2Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces2));
    pieces2Bitmap->Move(200, 150);

    pieces3 = img::GetImageAndScale(IMGPATH + "icons/black_horse.png", 1.8);
    pieces3Bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(pieces3));
    pieces3Bitmap->Move(300, 150);

    pieces1Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces1Click, this);
    pieces2Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces2Click, this);
    pieces3Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces3Click, this);

    btnBack = new wxButton(this, wxID_ANY, "<-",wxPoint(100,500));
    btnSave = new wxButton(this, wxID_ANY, "Save",wxPoint(200,500));

    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, "Configurazione attuale (quella sotto)", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    staticText->SetPosition(wxPoint(150, 250)); // Imposta la posizione

    selectedChessboard = chessboard3;
    chessboardSelectedBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(selectedChessboard));
    chessboardSelectedBitmap->Move(300, 300);

    selectedPieces = pieces1;
    selectedPiecesBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(selectedPieces));
    selectedPiecesBitmap->Move(150, 300);

    // TODO: Scacchiera di riferimento per vedere la configurazione con tutti i pezzi della scacchiera
}

void SettingsPanel::Chessboard1Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(chessboard1,chessboard1Bitmap);
}
void SettingsPanel::Chessboard2Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(chessboard2, chessboard2Bitmap);
}
void SettingsPanel::Chessboard3Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(chessboard3, chessboard3Bitmap);
}

void SettingsPanel::setSelectedChessboardBitmap(wxImage image, wxStaticBitmap* bitmap){
    selectedChessboard = image;
    chessboardSelectedBitmap->SetBitmap(bitmap->GetBitmap());
}

void SettingsPanel::setSelectedPiecesBitmap(wxImage image, wxStaticBitmap* bitmap){
    selectedPieces = image;
    selectedPiecesBitmap->SetBitmap(bitmap->GetBitmap());
}

void SettingsPanel::Pieces1Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(pieces1, pieces1Bitmap);
}

void SettingsPanel::Pieces2Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(pieces2, pieces2Bitmap);
}

void SettingsPanel::Pieces3Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(pieces3, pieces3Bitmap);
}

wxButton* SettingsPanel::GetBtnBack() { return btnBack; }
wxButton* SettingsPanel::GetBtnSave() { return btnSave; }

//  TODO
//   void SettingsPanel::SetSettingsConfig(SettingsConfig conf);
//   SettingsConfig SettingsPanel::GetSettingsConfig();
