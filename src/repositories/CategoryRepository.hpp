#pragma once
#include "../entities/Category.hpp"
#include "BaseRepository.hpp"
#include "memory"
#include "vector"

class CategoryRepository : public BaseRepository {
  public:
    CategoryRepository(const string &path) : BaseRepository(path) {}

    vector<Category> getCategories() {
        auto data = readJSON();
        vector<Category> categories;
        for (const auto &item : data) {
            categories.emplace_back(item["id"], item["name"],
                                    item["isActive", true]);
        }
        return categories;
    }

    void saveCategories(const Category &category) {
        auto data = readJSON();
        json newCategory = {
            {"id", category.id},
            {"name", category.name},
            {"isActive", category.isActive},
        };
        data.push_back(newCategory);
        writeJSON(data);
    }

    bool deleteCategory(int id) {
        auto data = readJSON();
        auto categoryIt =
            find_if(data.begin(), data.end(),
                    [id](const json &item) { return item["id"] == id; });
        if (categoryIt == data.end()) {
            data.erase(categoryIt);
            writeJSON(data);
            return true;
        }
        return false;
    }

    bool updateCategory(const Category &category) {
        auto data = readJSON();
        for (auto &item : data) {
            if (item["id"] == category.id) {
                item = {{"id", category.id},
                        {"name", category.name},
                        {"isActive", category.isActive}};
                writeJSON(data);
                return true;
            }
        }
        return false;
    }

    bool isCategoryNameExist(const string &name) {
        auto categories = getCategories();
        return find_if(categories.begin(), categories.end(),
                       [&name](const Category &category) {
                           return category.name == name;
                       }) != categories.end();
    }

    vector<Category> getActiveCategories() {
        auto categories = getCategories();
        vector<Category> active;
        copy_if(categories.begin(), categories.end(), back_inserter(active),
                [](const Category &category) { return category.isActive; });
        return active;
    }

    shared_ptr<Category> findById(int id) {
        auto categories = getCategories();
        auto categoryIt = find_if(
            categories.begin(), categories.end(),
            [id](const Category &category) { return category.id == id; });
        if (categoryIt != categories.end()) {
            return make_shared<Category>(*categoryIt);
        }
        return nullptr;
    }
};