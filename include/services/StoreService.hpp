#pragma once
#include "repositories/StoreRepository.hpp"

class StoreService {
  private:
    StoreRepository &storeRepostiory;

  public:
    StoreService(StoreRepository &storeRepostiory);
    bool updateStore(Store &store);
    vector<Store> viewStoreInfo();
};