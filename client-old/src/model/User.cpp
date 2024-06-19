#include "User.h"

User::User(std::string username, std::string name, std::string surname,unsigned short eloPoints) {
    this->username = username;
    this->name = name;
    this->surname = surname;
    this->eloPoints=eloPoints;
}
std::string User::Username() { return username;}
std::string User::Name() { return name;}
std::string User::Surname() { return surname;}
unsigned short User::EloPoints() {return eloPoints;}
void User::SetUsername(std::string username) {this->username=username;}
void User::SetName(std::string name) {this->name=name;}
void User::SetSurname(std::string surname) {this->surname=surname;}
void User::SetEloPoints(unsigned short EloPoints) {eloPoints=EloPoints;}