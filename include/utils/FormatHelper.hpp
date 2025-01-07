#pragma once
#include "iomanip"
#include "iostream"
#include "sstream"
#include "string"

using namespace std;

class FormatHelper {
  public:
    static string displayCurrency(double amount);
    static string formatDate(const time_t &timestamp);
    static void displayMessage(const string &type, const string &message);

    static void handleInvalidOption();
    static void handleInvalidInput();
};