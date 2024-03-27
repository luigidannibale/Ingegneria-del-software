#include "GameplayController.h"

GameplayController::GameplayController() {
    frame = new GameplayFrame();

    // frame->GetBoard()->Bind(wxEVT_LEFT_DOWN, &GameplayController::ClickBoard, this);
}


GameplayController::~GameplayController(){

}

void GameplayController::ClickBoard(wxMouseEvent& event) {
    wxPoint pos = event.GetPosition();
    // wxLogMessage("Mouse coordinates: (%d, %d)", pos.x, pos.y);

    int riga = pos.y / 80;
    int colonna = pos.x / 80;
    printf("Colonna %d - Riga %d\n", colonna,riga);

    const char* y[] = {"12345678","87654321"};
    const char x[] = "abcdefgh";

    printf("From controller Coordinate %c-%d \n",x[colonna],y[0][riga]-'0');

    // piece not clicked -> piece clicked -> (If your turn) move -> Piece not clicked
    // Check if piece already clicked -> move

    // If not, set piece clicked
}