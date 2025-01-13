#include "repositories/StoreRepository.hpp"

StoreRepository::StoreRepository(const string &path) : BaseRepository(path) {}

bool StoreRepository::updateStore(Store &store) {
    try {
        auto data = readJSON();
        data["storeName"] = store.getStoreName();
        data["ownerName"] = store.getOwnerName();
        data["description"] = store.getDescription();
        data["phoneNumber"] = store.getPhoneNumber();

        writeJSON(data);
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::cout << "[Error]: Failed to update store data." << endl;
        return false;
    }
    return false;
}

vector<Store> StoreRepository::getStores() {
    try {
        auto data = readJSON();
        vector<Store> stores;
        stores.emplace_back(data["storeName"], data["ownerName"],
                            data["description"], data["phoneNumber"]);
        return stores;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        cout << "[Error]: Failed to get stores data." << endl;
        return {};
    }
}