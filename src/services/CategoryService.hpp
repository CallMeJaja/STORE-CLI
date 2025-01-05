#pragma once
#include "../repositories/CategoryRepository.hpp"
#include "memory"
#include "string"
#include "vector"

class CategoryService {
  private:
    CategoryRepository &categoryRepository;

  public:
    CategoryService(CategoryRepository &categoryRepository)
        : categoryRepository(categoryRepository) {}

    bool addCategory(const string &name) {
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

    bool updateCategory(int id, const string &name) {
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

    bool deleteCategory(int id) {
        return categoryRepository.deleteCategory(id);
    }

    vector<Category> getActiveCategories() {
        return categoryRepository.getActiveCategories();
    }

    shared_ptr<Category> findById(int id) {
        return categoryRepository.findById(id);
    }

    bool toggleCategoryStatus(int id) {
        auto category = categoryRepository.findById(id);
        if (!category)
            return false;

        category->isActive = !category->isActive;
        return categoryRepository.updateCategory(*category);
    }

    bool validateCategoryName(const string &name) {
        return !categoryRepository.isCategoryNameExist(name);
    }

    int getTotalCategories() {
        return categoryRepository.getCategories().size();
    }
    int getActiveCount() {
        return categoryRepository.getActiveCategories().size();
    }
};