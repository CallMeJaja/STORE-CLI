#pragma once
#include "string"

using namespace std;

class Store {
  private:
    string storeName;
    string ownerName;
    string description;
    string phoneNumber;

  public:
    Store(string storeName, string ownerName, string description,
          string phoneNumber);

    void setStoreName(const string &newName);
    void setOwnerName(const string &newName);
    void setDescription(const string &newDescription);
    void setPhoneNumber(string newPhoneNumber);

    string getStoreName();
    string getOwnerName();
    string getDescription();
    string getPhoneNumber();
};
