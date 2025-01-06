#include "services/CategoryService.hpp"

CategoryService::CategoryService(CategoryRepository &categoryRepository)
    : categoryRepository(categoryRepository) {}

bool CategoryService::addCategory(const string &name) {
    if (categoryRepository.isCategoryNameExist(name)) {
        return false;
    }

    auto categories = categoryRepository.getCategories();
    bool hasDefault = false;

    // Check for default category
    for (const auto &cat : categories) {
        if (cat.name == "All Products") {
            hasDefault = true;
            break;
        }
    }

    // Add default category is not exist
    if (!hasDefault) {
        Category defaultCategory(1, "All Products", true);
        defaultCategory.isDefault = true;
        categoryRepository.saveCategories(defaultCategory);
    }

    // Add new category
    int newId = categories.empty() ? 1 : categories.back().id + 1;
    Category newCategory(newId, name, true, false);
    categoryRepository.saveCategories(newCategory);
    return true;
}

bool CategoryService::updateCategory(int id, const string &name) {
    auto category = categoryRepository.findById(id);
    if (!category)
        return false;

    if (name != category->name &&
        categoryRepository.isCategoryNameExist(name)) {
        return false;
    }

    category->name = name;
    return categoryRepository.updateCategory(*category);
}

bool CategoryService::deleteCategory(int id) {
    auto category = categoryRepository.findById(id);
    if (!category)
        return false;

    if (category->isDefault)
        return false;

    return categoryRepository.deleteCategory(id);
}

vector<Category> CategoryService::getActiveCategories() {
    return categoryRepository.getActiveCategories();
}

shared_ptr<Category> CategoryService::findById(int id) {
    return categoryRepository.findById(id);
}

bool CategoryService::toggleCategoryStatus(int id) {
    auto category = categoryRepository.findById(id);
    if (!category)
        return false;

    if (category->isDefault)
        return false;

    category->isActive = !category->isActive;
    return categoryRepository.updateCategory(*category);
}

bool CategoryService::validateCategoryName(const string &name) {
    return !categoryRepository.isCategoryNameExist(name);
}

int CategoryService::getTotalCategories() {
    return categoryRepository.getCategories().size();
}

int CategoryService::getActiveCount() {
    return categoryRepository.getActiveCategories().size();
}
