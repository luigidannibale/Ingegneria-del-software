#include "SettingsPanel.h"


SettingsPanel::SettingsPanel(wxWindow* parent, SettingsConfiguration* configuration): wxPanel(parent) {
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118,150,86));

    wxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *leftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxPanel *rightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxPanel *border = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    border->SetBackgroundColour(wxColour(0,0,0));
    border->SetSize(10, 810);

    wxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    // leftSizer->SetBackgroundColour(wxColour(100,100,200));
    
    chessboard1 = img::GetImageAndScale(IMGPATH + "chessboard-brown.png", 0.1);
    chessboard1Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(chessboard1));
    // chessboard1Bitmap->Move(100, 10);

    chessboard2 = img::GetImageAndScale(IMGPATH + "chessboard-blue.png", 0.1);
    chessboard2Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(chessboard2));
    // chessboard2Bitmap->Move(200, 10);

    chessboard3 = img::GetImageAndScale(IMGPATH + "chessboard-black.png", 0.1);
    chessboard3Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(chessboard3));
    // chessboard3Bitmap->Move(300, 10);

    chessboard1Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard1Click, this);
    chessboard2Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard2Click, this);
    chessboard3Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Chessboard3Click, this);

    pieces1 = img::GetImageAndScale(IMGPATH + "icons/blackKnight.png", 1.44);
    pieces1Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(pieces1));
    // pieces1Bitmap->Move(100, 150);

    pieces2 = img::GetImageAndScale(IMGPATH + "icons/whiteKnight.png", 1.44);
    pieces2Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(pieces2));
    // pieces2Bitmap->Move(200, 150);

    pieces3 = img::GetImageAndScale(IMGPATH + "icons2/black_horse.png", 1.8);
    pieces3Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(pieces3));
    // pieces3Bitmap->Move(300, 150);

    pieces1Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces1Click, this);
    pieces2Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces2Click, this);
    pieces3Bitmap->Bind(wxEVT_LEFT_DOWN, &SettingsPanel::Pieces3Click, this);

    btnBack = new wxButton(leftPanel, wxID_ANY, "Back",wxDefaultPosition,wxDefaultSize);
    btnSave = new wxButton(leftPanel, wxID_ANY, "Save",wxDefaultPosition,wxDefaultSize);

    wxStaticText* staticText = new wxStaticText(rightPanel, wxID_ANY, "Configurazione attuale (quella sotto)", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    // staticText->SetPosition(wxPoint(150, 250)); // Imposta la posizione
    // selectedChessboard = chessboard3;
    chessboardSelectedBitmap = new wxStaticBitmap(rightPanel, wxID_ANY, wxNullBitmap);
    // chessboardSelectedBitmap->Move(300, 300);
    // selectedPieces = pieces1;
    selectedPiecesBitmap = new wxStaticBitmap(rightPanel, wxID_ANY, wxNullBitmap);
    // selectedPiecesBitmap->Move(150, 300);
    //-----------------------
    SetSettingsConfig(configuration);


    wxStaticText* chooseText = new wxStaticText(this, wxID_ANY, "Scegliere una configurazione", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    leftSizer->Add(chooseText, wxSizerFlags(0).Expand().Border(wxALL, 5));

    wxStaticText* chessboardText = new wxStaticText(this, wxID_ANY, "Tipo di scacchiera", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    leftSizer->Add(chessboardText, wxSizerFlags(0).Expand().Border(wxALL, 5));

    wxSizer *chessboardSizer = new wxBoxSizer(wxHORIZONTAL);
    chessboardSizer->Add(chessboard1Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    chessboardSizer->Add(chessboard2Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    chessboardSizer->Add(chessboard3Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    leftSizer->Add(chessboardSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));

    wxStaticText* piecesText = new wxStaticText(this, wxID_ANY, "Tipo di pezzi", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    leftSizer->Add(piecesText, wxSizerFlags(0).Expand().Border(wxALL, 5));

    wxSizer *piecesSizer = new wxBoxSizer(wxHORIZONTAL);
    piecesSizer->Add(pieces1Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    piecesSizer->Add(pieces2Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    piecesSizer->Add(pieces3Bitmap, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    leftSizer->Add(piecesSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));

    wxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(btnBack, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    btnSizer->Add(btnSave, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5));
    leftSizer->Add(btnSizer, wxSizerFlags(4).Expand().Border(wxALL, 5));

    rightSizer->Add(staticText, wxSizerFlags(0).Expand().Border(wxALL, 5));
    rightSizer->Add(chessboardSelectedBitmap, wxSizerFlags(0).Expand().Border(wxALL, 5));
    rightSizer->Add(selectedPiecesBitmap, wxSizerFlags(0).Expand().Border(wxALL, 5));

    leftPanel->SetSizer(leftSizer);
    rightPanel->SetSizer(rightSizer);

    mainSizer->Add(leftPanel, wxSizerFlags(1).Expand());
    mainSizer->Add(border, wxSizerFlags(0).Expand());
    mainSizer->Add(rightPanel, wxSizerFlags(1).Expand());
    SetSizer(mainSizer);
    Layout();
    // TODO: Scacchiera di riferimento per vedere la configurazione con tutti i pezzi della scacchiera
}

void SettingsPanel::SetSettingsConfig(SettingsConfiguration* configuration) {
    this->configuration= configuration;
    switch (configuration->GetChessboard()) {
        case Chessboard::Black:
            setSelectedChessboardBitmap(chessboard3,chessboard3Bitmap);
            break;
        case Chessboard::Blue:
            setSelectedChessboardBitmap(chessboard2,chessboard2Bitmap);
            break;
        case Chessboard::Brown:
            setSelectedChessboardBitmap(chessboard1,chessboard1Bitmap);
            break;
    }
    switch (configuration->GetPieces()) {
        case Pieces::p1:
            setSelectedPiecesBitmap(pieces1, pieces1Bitmap);
            break;
        case Pieces::p2:
            setSelectedPiecesBitmap(pieces2, pieces2Bitmap);
            break;
        case Pieces::p3:
            setSelectedPiecesBitmap(pieces3, pieces3Bitmap);
            break;
    }
}
SettingsConfiguration* SettingsPanel::GetSettingsConfig() {return configuration;}
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
    Layout();
}

void SettingsPanel::setSelectedPiecesBitmap(wxImage image, wxStaticBitmap* bitmap){
    selectedPieces = image;
    selectedPiecesBitmap->SetBitmap(bitmap->GetBitmap());
    Layout();
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
