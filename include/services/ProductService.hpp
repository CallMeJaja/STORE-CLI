#pragma once
#include "algorithm"
#include "repositories/CategoryRepository.hpp"
#include "repositories/ProductRepository.hpp"
#include "vector"

class ProductService {
  private:
    ProductRepository &productRepository;
    CategoryRepository &categoryRepository;

  public:
    // Product Management
    ProductService(ProductRepository &productRepository,
                   CategoryRepository &categoryRepository);
    bool addProduct(const string &name, int price, const string &description,
                    const vector<string> &categories, int stock);
    bool updateProduct(int id, const string &name, int price,
                       const string &description, int stock);
    bool deleteProduct(int id);

    // Category Management
    bool addProductCategory(int productId, const string &category);
    bool removeProductCategory(int productId, const string &category);
    bool restockProduct(int productId, int amount);

    // Getter Product
    int getTotalProducts();
    vector<Product> getLowStockProducts(int threshold = 1);
    vector<Product> getProductsByCategory(const string &category);
    vector<Product> getProducts();
    vector<Product> getAvailableProducts();
};