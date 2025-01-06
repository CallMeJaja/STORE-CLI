#pragma once
#include "string"

using namespace std;

class User {
  public:
    int id;
    int balance;
    int totalTransactions;
    string fullName;
    string email;
    string password;
    string pin;
    bool isActive;
    bool isAdmin;
    User(int id, string fullName, string email, string password, string pin);
};