#ifndef USER_H
#define USER_H
#include <iostream>
#include <cstring>

class User
{
public:
    User();
    User(std::string username, std::string name, std::string surname, unsigned short eloPoints);
    std::string Username();
    std::string Name();
    std::string Surname();
    unsigned short EloPoints();
    void SetUsername(std::string username);
    void SetName(std::string name);
    void SetSurname(std::string surname);
    void SetEloPoints(unsigned short EloPoints);

private:
    std::string username;
    std::string name;
    std::string surname;
    unsigned short eloPoints;
};
#endif // USER_H
