
#ifndef GAMEPLAYFRAME_H
#define GAMEPLAYFRAME_H

#include <wx-3.0/wx/wx.h>
#include <string>
#include "ImageFuncions.h"
#include "ChessboardView.h"

class GameplayFrame : public wxFrame{
public:
    GameplayFrame();

    void StartGame();
    wxStaticBitmap* GetBoard();


private:
    const int chessX = 50;
    const int chessY = 50;
    std::string const IMGPATH = "../resources/img/";

    ChessboardView* chessboard;


    wxStaticText* opponentText;
    wxStaticText* userText;

    void createPieces(wxStaticBitmap**, int, int, int);
};


#endif //GAMEPLAYFRAME_H
