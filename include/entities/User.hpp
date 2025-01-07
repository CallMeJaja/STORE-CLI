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
    User(int id, int totalTransactions, string fullName, string email,
         string password, string pin, int balance, bool isActive, bool isAdmin);
    // User(int id, string fullName, string email, string password, string pin,
    //      bool isActive, bool isAdmin, int totalTransactions = 0,
    //      int balance = 0);
};