#pragma once
#include "BaseRepository.hpp"
#include "entities/Product.hpp"
#include "fstream"
#include "iostream"
#include "memory"
#include "nlohmann/json.hpp"
#include "vector"

using json = nlohmann::json;
using namespace std;

class ProductRepository : public BaseRepository {
  public:
    ProductRepository(const string &path);

    bool deleteProduct(int id);
    bool updateProduct(const Product &product);
    bool saveProduct(const Product &product);
    bool productIsExist(int id);

    vector<Product> getProductsByCategory(const string &category);
    vector<Product> getProducts();
    shared_ptr<Product> findById(int id);
};