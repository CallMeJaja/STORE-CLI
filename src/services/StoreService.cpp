#include "services/StoreService.hpp"

StoreService::StoreService(StoreRepository &storeRepostiory)
    : storeRepostiory(storeRepostiory) {}

bool StoreService::updateStore(Store &store) {
    return storeRepostiory.updateStore(store);
}

vector<Store> StoreService::viewStoreInfo() {
    return storeRepostiory.getStores();
}