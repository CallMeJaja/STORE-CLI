#include "services/ProductService.hpp"

ProductService::ProductService(ProductRepository &productRepository,
                               CategoryRepository &categoryRepository)
    : productRepository(productRepository),
      categoryRepository(categoryRepository) {}

bool ProductService::addProduct(const string &name, int price,
                                const string &description,
                                const vector<string> &categories, int stock) {

    auto products = productRepository.getProducts();
    int newId = products.empty() ? 1 : products.back().id + 1;

    Product newProduct(newId, name, price, description, categories, stock);
    return productRepository.saveProduct(newProduct);
}

bool ProductService::updateProduct(int id, const string &name, int price,
                                   const string &description, int stock) {
    auto product = productRepository.findById(id);
    if (!product)
        return false;

    product->name = name;

    if (!product->setPrice(price)) {
        return false;
    }

    if (!product->setStock(stock)) {
        return false;
    }

    product->description = description;
    return productRepository.updateProduct(*product);
}

bool ProductService::deleteProduct(int id) {
    return productRepository.deleteProduct(id);
}

bool ProductService::addProductCategory(int productId, const string &category) {
    auto product = productRepository.findById(productId);
    if (!product)
        return false;

    product->addCategory(category);
    return productRepository.updateProduct(*product);
}

bool ProductService::removeProductCategory(int productId,
                                           const string &category) {
    auto product = productRepository.findById(productId);
    if (!product)
        return false;

    product->removeCategory(category);
    return productRepository.updateProduct(*product);
}

bool ProductService::restockProduct(int productId, int amount) {
    auto product = productRepository.findById(productId);
    if (!product)
        return false;

    product->restockProduct(amount);
    return productRepository.updateProduct(*product);
}

int ProductService::getTotalProducts() {
    return productRepository.getProducts().size();
}

vector<Product> ProductService::getLowStockProducts(int threshold) {
    auto products = productRepository.getProducts();
    vector<Product> lowStockProducts;

    copy_if(products.begin(), products.end(), back_inserter(lowStockProducts),
            [&](Product &p) { return p.isLowStock(threshold); });

    return lowStockProducts;
}

vector<Product> ProductService::getProductsByCategory(const string &category) {
    return productRepository.getProductsByCategory(category);
}

vector<Product> ProductService::getProducts() {
    return productRepository.getProducts();
}

vector<Product> ProductService::getAvailableProducts() {
    auto products = productRepository.getProducts();
    vector<Product> availableProducts;

    copy_if(products.begin(), products.end(), back_inserter(availableProducts),
            [&](Product &p) { return p.isAvailable(); });

    return availableProducts;
}
