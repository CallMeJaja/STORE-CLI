#pragma once
#include "memory"
#include "repositories/CategoryRepository.hpp"
#include "string"
#include "vector"

class CategoryService {
  private:
    CategoryRepository &categoryRepository;

  public:
    CategoryService(CategoryRepository &categoryRepository);

    bool addCategory(const string &name);
    bool updateCategory(int id, const string &name);
    bool deleteCategory(int id);

    vector<Category> getActiveCategories();
    shared_ptr<Category> findById(int id);

    bool toggleCategoryStatus(int id);
    bool validateCategoryName(const string &name);

    int getTotalCategories();
    int getActiveCount();
};