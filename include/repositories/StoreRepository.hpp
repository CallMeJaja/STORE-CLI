#pragma once
#include "BaseRepository.hpp"
#include "entities/Store.hpp"
#include "iostream"
#include "vector"

class StoreRepository : public BaseRepository {
  public:
    StoreRepository(const string &path);

    bool updateStore(Store &store);
    vector<Store> getStores();
};