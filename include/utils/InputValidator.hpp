#pragma once
#include "FormatHelper.hpp"
#include "algorithm"
#include "iostream"
#include "string"
#include "unistd.h"

using namespace std;

class InputValidator {
  public:
    static bool validateStringInput(string &input, const string &prompt);
    static bool validateIntInput(int &input, const string &prompt,
                                 int menuSize = 0);
    static bool validatePassword(const string &password);
    static bool validatePin(const string &pin);
    static bool validateEmail(const string &email);
    static bool validateConfirmation(const string &prompt);
    static void clearInputBuffer();
};