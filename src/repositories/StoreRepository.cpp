#include "repositories/StoreRepository.hpp"

StoreRepository::StoreRepository(const string &path) : BaseRepository(path) {}

bool StoreRepository::updateStore(Store &store) {
    try {
        auto data = readJSON();
        for (auto &item : data) {
            item = {{"storeName", store.getStoreName()},
                    {"ownerName", store.getOwnerName()},
                    {"description", store.getDescription()},
                    {"phoneNumber", store.getPhoneNumber()}};
            writeJSON(data);
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::cout << "[Error]: Failed to update store data." << endl;
        return false;
    }
}

vector<Store> StoreRepository::getStores() {
    try {
        auto data = readJSON();
        vector<Store> stores;
        for (const auto &item : data) {
            stores.emplace_back(item["storeName"], item["ownerName"],
                                item["description"], item["phoneNumber"]);
        }
        return stores;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        cout << "[Error]: Failed to get stores data." << endl;
    }
}