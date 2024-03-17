#include "GameOptions.h"
#include <stdio.h>
#include <utility>
#include <iostream>

GameTime::GameTime(int duration) : GameTime(duration, 0) {}

GameTime::GameTime(int duration, int increment) {
    this->duration = duration;
    this->increment = increment;
}

int GameTime::GetDurationInSeconds() {return duration;}
int GameTime::GetIncrement() {return increment;}

GameOptions::GameOptions(bool againstHuman,GameTime* gameTime, ComputerElo computerElo, StartSide startSide) {
    this->againtsHuman = againstHuman;
    this->computerElo = computerElo;
    this->startSide = startSide;
    this->gameTime = gameTime;
}

GameOptions::GameOptions(bool againstHuman,int duration, int increment, ComputerElo computerElo, StartSide startSide)
    : GameOptions(againstHuman,new GameTime(duration,increment),computerElo,startSide) {}

StartSide GameOptions::GetStartSide() {return startSide;}
GameTime *GameOptions::GetGameTime() {return gameTime;}
ComputerElo GameOptions::GetComputerElo() {return computerElo;}
bool GameOptions::GetAgaintsHuman() {return againtsHuman;}
int GameOptions::GetGameDurationInSeconds() {return gameTime->GetDurationInSeconds();}
int GameOptions::GetGameIncrement() {return gameTime->GetIncrement();}