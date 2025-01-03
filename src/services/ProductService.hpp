#pragma once
#include "../repositories/CategoryRepository.hpp"
#include "../repositories/ProductRepository.hpp"
#include "algorithm"
#include "vector"

class ProductService {
  private:
    ProductRepository &productRepository;
    CategoryRepository &categoryRepository;

  public:
    ProductService(ProductRepository &productRepository,
                   CategoryRepository &categoryRepository)
        : productRepository(productRepository),
          categoryRepository(categoryRepository) {}

    bool addProduct(const string &name, int price, const string &description,
                    const vector<string> &categories, int stock) {
        if (price <= 0 || stock < 0)
            return false;

        auto products = productRepository.getProducts();
        int newId = products.empty() ? 1 : products.back().id + 1;

        Product newProduct(newId, name, price, description, categories, stock);
        productRepository.saveProduct(newProduct);
        return true;
    }

    bool updateProduct(int id, const string &name, int price,
                       const string &description, int stock) {
        auto product = productRepository.findById(id);
        if (!product)
            return false;

        product->name = name;
        if (!product->setPrice(price))
            return false;
        if (!product->setStock(stock))
            return false;
        product->description = description;

        return productRepository.updateProduct(*product);
    }

    bool deleteProduct(int id) { return productRepository.deleteProduct(id); }

    vector<Product> getLowStockProducts(int threshold = 1) {
        auto products = productRepository.getProducts();
        vector<Product> lowStock;

        copy_if(
            products.begin(), products.end(), back_inserter(lowStock),
            [threshold](const Product &p) { return p.isLowStock(threshold); });

        return lowStock;
    }

    vector<Product> getProductsByCategory(const string &category) {
        return productRepository.getProductsByCategory(category);
    }

    vector<Product> getProducts() { return productRepository.getProducts(); }

    bool addProductCategory(int productId, const string &category) {
        auto product = productRepository.findById(productId);
        if (!product)
            return false;

        product->addCategory(category);
        return productRepository.updateProduct(*product);
    }

    bool removeProductCategory(int productId, const string &category) {
        auto product = productRepository.findById(productId);
        if (!product)
            return false;

        product->removeCategory(category);
        return productRepository.updateProduct(*product);
    }

    bool restockProduct(int productId, int amount) {
        if (amount <= 0)
            return false;

        auto product = productRepository.findById(productId);
        if (!product)
            return false;

        product->restockProduct(amount);
        return productRepository.updateProduct(*product);
    }

    int getTotalProducts() { return productRepository.getProducts().size(); }

    vector<Product> getAvailableProducts() {
        auto products = productRepository.getProducts();
        vector<Product> available;

        copy_if(products.begin(), products.end(), back_inserter(available),
                [](const Product &p) { return p.isAvailable(); });

        return available;
    }
};