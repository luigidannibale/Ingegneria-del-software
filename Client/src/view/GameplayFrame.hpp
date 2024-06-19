#ifndef GAMEPLAYFRAME_H
#define GAMEPLAYFRAME_H

#include <string>
#include <atomic>
#include "../lib.hpp"
#include "ImageFuncions.hpp"
#include "ChessboardView.hpp"
#include "../model/GameOptions.hpp"
#include "GameGraphicOptions.hpp"
#include <wx/listctrl.h>
#include <thread>


class GameplayFrame : public wxFrame{
public:
    GameplayFrame(bool, GameOptions*, GameGraphicOptions*);
    ~GameplayFrame();

    void StopUpdateTimer();
    void ChangeTimer();
    void StartTimer();
    void ShowTransparentPanel();
    void HideTransparentPanel();
    void UpdateTransparentPanel(std::string);
    void AddMoveToList(std::string );
    wxStaticBitmap* GetBoard();
    ChessboardView* GetChessboard();

    int UpdateTime(wxTimerEvent& event);


private:
    float chessX = 50;
    float chessY = 90;
    bool whiteMove = true;

    std::atomic_int whiteSeconds;
    std::atomic_int blackSeconds;

    int increment;
    wxTimer* whiteTimer;
    wxTimer* blackTimer;


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

    std::string secondsToString(int seconds);
};


#endif //GAMEPLAYFRAME_H
