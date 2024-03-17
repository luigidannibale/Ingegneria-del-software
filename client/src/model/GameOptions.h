#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

enum class ComputerElo : int{
    Beginner = 0,
    Intermediate = 1,
    Advanced = 2,
    Master = 3
};

enum class StartSide : int{
    Casual = 0,
    White,
    Black,
};

class GameTime {
public:
    GameTime(int);
    GameTime(int, int);
    int GetDurationInSeconds();
    int GetIncrement();


private:
    // In seconds
    int duration;
    int increment;
};


struct GameOptions {
public:
    GameOptions(bool,int, int, ComputerElo, StartSide);
    GameOptions(bool,GameTime*, ComputerElo, StartSide);
    ComputerElo GetComputerElo();
    StartSide GetStartSide();
    GameTime* GetGameTime();
    int GetGameDurationInSeconds();
    int GetGameIncrement();
    bool GetAgaintsHuman();
private:
    bool againtsHuman;
    enum ComputerElo computerElo;
    enum StartSide startSide;
    GameTime* gameTime;
};



#endif //GAMEOPTIONS_H
