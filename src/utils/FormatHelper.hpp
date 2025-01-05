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

    static void displayMessage(const string &type, const string &message) {
        if (type == "success") {
            std::cout << "[Success]: " << message << endl;
        } else if (type == "error") {
            std::cout << "[Error]: " << message << endl;
        } else if (type == "info") {
            std::cout << "[Info]: " << message << endl;
        }
    }

    static void handleInvalidOption() {
        displayMessage("error", "Invalid option. Please try again.");
    }

    static void handleInvalidInput() {
        displayMessage("error", "Invalid input. Please try again.");
    }
};