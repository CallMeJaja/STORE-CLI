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
};