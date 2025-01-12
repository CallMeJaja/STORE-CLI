#include "entities/Store.hpp"

Store::Store(string storeName, string ownerName, string description,
             int phoneNumber)
    : storeName(storeName), ownerName(ownerName), description(description),
      phoneNumber(phoneNumber) {}

void Store::setStoreName(const string &newName) { storeName = newName; }

void Store::setOwnerName(const string &newName) { ownerName = newName; }

void Store::setDescription(const string &newDescription) {
    description = newDescription;
}

void Store::setPhoneNumber(int newPhoneNumber) { phoneNumber = newPhoneNumber; }

string Store::getStoreName() { return storeName; }

string Store::getOwnerName() { return ownerName; }

string Store::getDescription() { return description; }

int Store::getPhoneNumber() { return phoneNumber; }