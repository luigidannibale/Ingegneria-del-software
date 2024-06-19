#include "SettingsPanel.h"


SettingsPanel::SettingsPanel(wxWindow* parent, GameGraphicOptions* graphicOptions): wxPanel(parent) {
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

    pieces1 = img::GetImageAndScale(IMGPATH + "icons_neo/blackKnight.png", 1.44);
    pieces1Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(pieces1));
    // pieces1Bitmap->Move(100, 150);

    pieces2 = img::GetImageAndScale(IMGPATH + "icons_neo/whiteKnight.png", 1.44);
    pieces2Bitmap = new wxStaticBitmap(leftPanel, wxID_ANY, wxBitmap(pieces2));
    // pieces2Bitmap->Move(200, 150);

    pieces3 = img::GetImageAndScale(IMGPATH + "icons_pixel/whiteKnight.png", 1.44);
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
    this->graphicOptions = graphicOptions;
    SetGameGraphicOptions(graphicOptions);


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

void SettingsPanel::SetGameGraphicOptions(GameGraphicOptions* graphicOptions) {
    // this->graphicOptions= graphicOptions;
    switch (graphicOptions->GetBoardStyle()) {
        case BoardStyle::black:
            setSelectedChessboardBitmap(BoardStyle::black,chessboard3Bitmap);
            break;
        case BoardStyle::blue:
            setSelectedChessboardBitmap(BoardStyle::blue,chessboard2Bitmap);
            break;
        case BoardStyle::brown:
            setSelectedChessboardBitmap(BoardStyle::brown,chessboard1Bitmap);
            break;
    }
    switch (graphicOptions->GetPiecesStyle()) {
        case PiecesStyle::neo:
            setSelectedPiecesBitmap(PiecesStyle::neo, pieces1Bitmap);
            break;
        case PiecesStyle::neo2:
            setSelectedPiecesBitmap(PiecesStyle::neo2, pieces2Bitmap);
            break;
        case PiecesStyle::pixel:
            setSelectedPiecesBitmap(PiecesStyle::pixel, pieces3Bitmap);
            break;
    }
}

GameGraphicOptions* SettingsPanel::GetGameGraphicOptions() {return graphicOptions;}

void SettingsPanel::Chessboard1Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(BoardStyle::brown,chessboard1Bitmap);
}
void SettingsPanel::Chessboard2Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(BoardStyle::blue, chessboard2Bitmap);
}
void SettingsPanel::Chessboard3Click(wxMouseEvent&) {
    setSelectedChessboardBitmap(BoardStyle::black, chessboard3Bitmap);
}

void SettingsPanel::setSelectedChessboardBitmap(BoardStyle board, wxStaticBitmap* bitmap){
    // selectedChessboard = image;
    graphicOptions->SetBoardStyle(board);
    chessboardSelectedBitmap->SetBitmap(bitmap->GetBitmap());
    Layout();
}

void SettingsPanel::setSelectedPiecesBitmap(PiecesStyle pieces, wxStaticBitmap* bitmap){
    // selectedPieces = image;
    graphicOptions->SetPiecesStyle(pieces);
    selectedPiecesBitmap->SetBitmap(bitmap->GetBitmap());
    Layout();
}

void SettingsPanel::Pieces1Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(PiecesStyle::neo, pieces1Bitmap);
}

void SettingsPanel::Pieces2Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(PiecesStyle::neo2, pieces2Bitmap);
}

void SettingsPanel::Pieces3Click(wxMouseEvent& event) {
    setSelectedPiecesBitmap(PiecesStyle::pixel, pieces3Bitmap);
}

wxButton* SettingsPanel::GetBtnBack() { return btnBack; }
wxButton* SettingsPanel::GetBtnSave() { return btnSave; }

//  TODO
//   void SettingsPanel::SetSettingsConfig(SettingsConfig conf);
//   SettingsConfig SettingsPanel::GetSettingsConfig();
