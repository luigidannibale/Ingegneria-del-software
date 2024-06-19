#ifndef SETTINGSPANEL_H
#define  SETTINGSPANEL_H

#include "../lib.hpp"
#include "ImageFuncions.hpp"
#include "GameGraphicOptions.hpp"


class SettingsPanel : public wxPanel{
public:
    SettingsPanel(wxWindow*, GameGraphicOptions*);

    wxButton* GetBtnBack();
    wxButton* GetBtnSave();
    void SetGameGraphicOptions(GameGraphicOptions*);
    GameGraphicOptions* GetGameGraphicOptions();

private:
    GameGraphicOptions* graphicOptions;

    wxButton* btnBack;
    wxButton* btnSave;

    // wxImage selectedChessboard;
    // wxImage selectedPieces;
    // BoardStyle boardStyle;
    // PiecesStyle piecesStyle;
    wxStaticBitmap* chessboardSelectedBitmap;
    wxStaticBitmap* selectedPiecesBitmap;

    wxImage chessboard1;
    wxImage chessboard2;
    wxImage chessboard3;
    wxStaticBitmap* chessboard1Bitmap;
    wxStaticBitmap* chessboard2Bitmap;
    wxStaticBitmap* chessboard3Bitmap;

    wxImage pieces1;
    wxImage pieces2;
    wxImage pieces3;
    wxStaticBitmap* pieces1Bitmap;
    wxStaticBitmap* pieces2Bitmap;
    wxStaticBitmap* pieces3Bitmap;

    void Chessboard1Click(wxMouseEvent&);
    void Chessboard2Click(wxMouseEvent&);
    void Chessboard3Click(wxMouseEvent&);

    void Pieces1Click(wxMouseEvent&);
    void Pieces2Click(wxMouseEvent&);
    void Pieces3Click(wxMouseEvent&);

    void setSelectedChessboardBitmap(BoardStyle, wxStaticBitmap*);
    void setSelectedPiecesBitmap(PiecesStyle, wxStaticBitmap*);
};

#endif //SETTINGSPANEL_H


