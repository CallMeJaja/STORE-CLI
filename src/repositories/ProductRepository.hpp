#pragma once
#include "../../nlohmann/json.hpp"
#include "../entities/Product.hpp"
#include "BaseRepository.hpp"
#include "fstream"
#include "vector"

using json = nlohmann::json;
using namespace std;

class ProductRepository : public BaseRepository {
  public:
    ProductRepository(const string &path) : BaseRepository(path) {}

    vector<Product> getProductsByCategory(const string &category) {
        auto products = getProducts();
        vector<Product> filtered;

        copy_if(products.begin(), products.end(), back_inserter(filtered),
                [&category](const Product &product) {
                    return find(product.categories.begin(),
                                product.categories.end(),
                                category) != product.categories.end();
                });
        return filtered;
    }

    bool deleteProduct(int id) {
        auto data = readJSON();
        auto productIt =
            find_if(data.begin(), data.end(),
                    [id](const json &item) { return item["id"] == id; });

        if (productIt != data.end()) {
            data.erase(productIt);
            writeJSON(data);
            return true;
        }
        return false;
    }

    bool updateProduct(const Product &product) {
        auto data = readJSON();
        for (auto &item : data) {
            if (item["id"] == product.id) {
                item = {
                    {"id", product.id},
                    {"name", product.name},
                    {"price", product.price},
                    {"description", product.description},
                    {"categories", product.categories},
                    {"stock", product.stock},
                    {"sold", product.sold},
                };
                writeJSON(data);
                return true;
            }
        }
    }

    vector<Product> getProducts() {
        auto data = readJSON();
        vector<Product> products;
        for (const auto &item : data) {
            products.emplace_back(item["id"], item["name"], item["price"],
                                  item["description"], item["category"],
                                  item["stock"], item["sold"]);
        }
        return products;
    }

    void saveProduct(const Product &product) {
        auto data = readJSON();
        json newProduct = {{"id", product.id},
                           {"name", product.name},
                           {"price", product.price},
                           {"description", product.description},
                           {"categories", product.categories},
                           {"stock", product.stock},
                           {"sold", product.sold}};
        data.push_back(newProduct);
        writeJSON(data);
    }

    Product *findById(int id) {
        auto products = getProducts();
        auto productIt =
            find_if(products.begin(), products.end(),
                    [id](const Product &product) { return product.id == id; });
        return productIt != products.end() ? &(*productIt) : nullptr;
    }

    bool exists(int id) {
        auto data = readJSON();
        return find_if(data.begin(), data.end(), [id](const json &item) {
                   return item["id"] == id;
               }) != data.end();
    }
};