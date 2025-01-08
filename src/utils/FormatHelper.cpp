#include "utils/FormatHelper.hpp"

string FormatHelper::displayCurrency(double amount) {
    stringstream ss;
    ss << fixed << setprecision(0) << amount;

    string result = ss.str();
    int insertPosition = result.length() - 3;

    while (insertPosition > 0) {
        result.insert(insertPosition, ".");
        insertPosition -= 3;
    }

    return "Rp. " + result;
}

string FormatHelper::formatDate(const time_t &timestamp) {
    char buffer[80];
    struct tm *timeinfo = localtime(&timestamp);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

void FormatHelper::displayMessage(const string &type, const string &message) {
    if (type == "success") {
        std::cout << "[Success]: " << message << endl;
    } else if (type == "error") {
        std::cout << "[Error]: " << message << endl;
    } else if (type == "info") {
        std::cout << "[Info]: " << message << endl;
    }
}

void FormatHelper::handleInvalidOption() {
    displayMessage("error", "Invalid option. Please try again.");
}

void FormatHelper::handleInvalidInput() {
    displayMessage("error", "Invalid input. Please try again.");
}
