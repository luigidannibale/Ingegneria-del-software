
#ifndef GAMEPLAYFRAME_H
#define GAMEPLAYFRAME_H

#include <string>
#include <atomic>
#include "../lib.h"
#include "ImageFuncions.h"
#include "ChessboardView.h"
#include "../model/GameOptions.h"
#include <wx/listctrl.h>


class GameplayFrame : public wxFrame{
public:
    GameplayFrame(bool, GameOptions*);
    ~GameplayFrame();

    void StartGame();
    void StopUpdateTimer();
    void ChangeTimer();
    void StartTimer();
    void ShowTransparentPanel();
    void HideTransparentPanel();
    void UpdateTransparentPanel(std::string);
    wxStaticBitmap* GetBoard();
    ChessboardView* GetChessboard();


private:
    float chessX = 50;
    float chessY = 90;

    std::string const IMGPATH = "../resources/img/";
    std::atomic_int whiteSeconds;
    std::atomic_int blackSeconds;

    int increment;
    wxTimer* whiteTimer;
    wxTimer* blackTimer;
    bool runningUpdate = false;

    ChessboardView* chessboard;


    wxStaticText* opponentText;
    wxStaticText* userText;
    wxStaticText* whiteStatText;
    wxStaticText* blackStatText;
    wxStaticText* whiteTimerText;
    wxStaticText* blackTimerText;

    wxListView* movesPlayedList;
    wxPanel* transparentPanel;
    wxTextCtrl* loadingText;

    void createPieces(wxStaticBitmap**, int, int, int);

    void UpdateTime(wxTimerEvent& event);
    void UpdateWhiteTimer(wxTimerEvent& event);
    void UpdateBlackTimer(wxTimerEvent& event);
    std::string secondsToString(int seconds);
};


#endif //GAMEPLAYFRAME_H
