#ifndef SETTINGSPANEL_H
#define  SETTINGSPANEL_H

#include <wx-3.0/wx/wx.h>
#include "ImageFuncions.h"
#include "../model/SettingsConfiguration.h"

class SettingsPanel : public wxPanel{
public:
    SettingsPanel(wxWindow*, SettingsConfiguration*);

    wxButton* GetBtnBack();
    wxButton* GetBtnSave();
    void SetSettingsConfig(SettingsConfiguration* configuration);
    SettingsConfiguration* GetSettingsConfig();

private:

    SettingsConfiguration* configuration;

    wxButton* btnBack;
    wxButton* btnSave;

    wxImage selectedChessboard;
    wxImage selectedPieces;
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
    std::string const IMGPATH = "../resources/img/";

    void Chessboard1Click(wxMouseEvent&);
    void Chessboard2Click(wxMouseEvent&);
    void Chessboard3Click(wxMouseEvent&);

    void Pieces1Click(wxMouseEvent&);
    void Pieces2Click(wxMouseEvent&);
    void Pieces3Click(wxMouseEvent&);

    void setSelectedChessboardBitmap(wxImage, wxStaticBitmap*);
    void setSelectedPiecesBitmap(wxImage, wxStaticBitmap*);
};

#endif //SETTINGSPANEL_H


