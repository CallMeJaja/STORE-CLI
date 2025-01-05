#pragma once
#include "../entities/Category.hpp"
#include "BaseRepository.hpp"
#include "iostream"
#include "memory"
#include "vector"

class CategoryRepository : public BaseRepository {
  public:
    CategoryRepository(const string &path) : BaseRepository(path) {}

    vector<Category> getCategories() {
        auto data = readJSON();
        vector<Category> categories;
        for (const auto &item : data) {
            try {
                categories.emplace_back(item["id"], item["name"],
                                        item["isActive"], item["isDefault"]);
            } catch (...) {
                std::cout << "[Error]: Failed to parse category data." << endl;
            }
        }
        return categories;
    }

    void saveCategories(const Category &category) {
        auto data = readJSON();
        json newCategory = {
            {"id", category.id},
            {"name", category.name},
            {"isActive", category.isActive},
            {"isDefault", category.isDefault},
        };
        data.push_back(newCategory);
        writeJSON(data);
    }

    bool deleteCategory(int id) {
        auto data = readJSON();
        auto categoryIt =
            find_if(data.begin(), data.end(),
                    [id](const json &item) { return item["id"] == id; });
        cout << "[DEBUG] ID " << id << endl;
        cout << data.dump(4) << endl;
        system("pause");

        if (categoryIt != data.end()) {
            data.erase(categoryIt);

            for (int i = 1; i < data.size(); i++) {
                data[i]["id"] = i + 1;
            }

            writeJSON(data);
            return true;
        }
        return false;
    }

    bool updateCategory(const Category &category) {
        if (category.isDefault)
            return false; // Prevent updating default category

        auto data = readJSON();
        for (auto &item : data) {
            if (item["id"] == category.id) {
                item = {{"id", category.id},
                        {"name", category.name},
                        {"isActive", category.isActive},
                        {"isDefault", category.isDefault}};
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