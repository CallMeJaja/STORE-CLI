#pragma once
#include "BaseRepository.hpp"
#include "entities/Category.hpp"
#include "iostream"
#include "memory"
#include "vector"

class CategoryRepository : public BaseRepository {
  public:
    CategoryRepository(const string &path);

    bool saveCategories(const Category &category);
    bool deleteCategory(int id);
    bool updateCategory(const Category &category);
    bool isCategoryNameExist(const string &name);

    vector<Category> getCategories();
    vector<Category> getActiveCategories();
    shared_ptr<Category> findById(int id);
};