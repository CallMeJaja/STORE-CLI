#pragma once
#include "iomanip"
#include "sstream"
#include "string"

using namespace std;

class FormatHelper {
  public:
    static string displayCurrency(double amount) {
        stringstream ss;
        ss << fixed << setprecision(0) << amount;

        string result = ss.str();
        int inseretPosition = result.length() - 3;

        while (inseretPosition > 0) {
            result.insert(inseretPosition, ".");
            inseretPosition -= 3;
        }

        return "Rp. " + result;
    }

    static string formatDate(const time_t &timestamp) {
        struct tm *timeInfo;
        char buffer[80];

        timeInfo = localtime(&timestamp);
        strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeInfo);

        return string(buffer);
    }
};