#include "repositories/ProductRepository.hpp"

ProductRepository::ProductRepository(const string &path)
    : BaseRepository(path) {}

vector<Product>
ProductRepository::getProductsByCategory(const string &category) {
    auto products = getProducts();
    vector<Product> filteredProducts;

    copy_if(products.begin(), products.end(), back_inserter(filteredProducts),
            [&category](const Product &product) {
                return find(product.categories.begin(),
                            product.categories.end(),
                            category) != product.categories.end();
            });
    return filteredProducts;
}

bool ProductRepository::deleteProduct(int id) {
    auto data = readJSON();
    auto productIt = find_if(data.begin(), data.end(), [id](const json &item) {
        return item["id"] == id;
    });
    if (productIt != data.end()) {
        data.erase(productIt);
        for (int i = 1; i < data.size(); i++) {
            data[i]["id"] = i + 1;
        }
        writeJSON(data);
        return true;
    }
    return false;
}

bool ProductRepository::updateProduct(const Product &product) {
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
    return false;
}

bool ProductRepository::saveProduct(const Product &product) {
    try {
        auto data = readJSON();
        json newProduct = {
            {"id", product.id},
            {"name", product.name},
            {"price", product.price},
            {"description", product.description},
            {"categories", product.categories},
            {"stock", product.stock},
            {"sold", product.sold},
        };
        data.push_back(newProduct);
        writeJSON(data);

    } catch (...) {
        return false;
    }
}

bool ProductRepository::productIsExist(int id) {
    auto products = getProducts();
    auto productIt =
        find_if(products.begin(), products.end(),
                [id](const Product &product) { return product.id == id; });
    return productIt != products.end();
}

vector<Product> ProductRepository::getProducts() {
    auto data = readJSON();
    vector<Product> products;
    for (const auto &item : data) {
        try {
            products.emplace_back(item["id"], item["name"], item["price"],
                                  item["description"], item["categories"],
                                  item["stock"], item["sold"]);
        } catch (...) {
            std::cout << "[Error]: Failed to parse product data." << endl;
        }
    }
    return products;
}

shared_ptr<Product> ProductRepository::findById(int id) {
    auto products = getProducts();
    auto productIt =
        find_if(products.begin(), products.end(),
                [id](const Product &product) { return product.id == id; });
    if (productIt != products.end()) {
        return make_shared<Product>(*productIt);
    }
    return nullptr;
}
