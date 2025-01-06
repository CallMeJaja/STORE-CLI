#include "repositories/CategoryRepository.hpp"

CategoryRepository::CategoryRepository(const string &path)
    : BaseRepository(path) {}

vector<Category> CategoryRepository::getCategories() {
    auto data = readJSON();
    vector<Category> categories;
    for (const auto &item : data) {
        try {
            categories.emplace_back(item["id"], item["name"], item["isActive"],
                                    item["isDefault"]);
        } catch (...) {
            std::cout << "[Error]: Failed to parse category data." << endl;
        }
    }
    return categories;
}

bool CategoryRepository::saveCategories(const Category &category) {
    try {
        auto data = readJSON();
        json newCategory = {
            {"id", category.id},
            {"name", category.name},
            {"isActive", category.isActive},
            {"isDefault", category.isDefault},
        };
        data.push_back(newCategory);
        writeJSON(data);
        return true;
    } catch (...) {
        return false;
    }
}

bool CategoryRepository::deleteCategory(int id) {
    auto data = readJSON();
    auto categoryIt = find_if(data.begin(), data.end(), [id](const json &item) {
        return item["id"] == id;
    });
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

bool CategoryRepository::updateCategory(const Category &category) {
    auto data = readJSON();
    for (auto &item : data) {
        if (item["id"] == category.id) {
            item = {
                {"id", category.id},
                {"name", category.name},
                {"isActive", category.isActive},
                {"isDefault", category.isDefault},
            };
            writeJSON(data);
            return true;
        }
    }
    return false;
}

bool CategoryRepository::isCategoryNameExist(const string &name) {
    auto categories = getCategories();
    auto categoryIt = find_if(
        categories.begin(), categories.end(),
        [&name](const Category &category) { return category.name == name; });
    return categoryIt != categories.end();
}

vector<Category> CategoryRepository::getActiveCategories() {
    auto categories = getCategories();
    vector<Category> activeCategories;

    copy_if(categories.begin(), categories.end(),
            back_inserter(activeCategories),
            [](const Category &category) { return category.isActive; });
    return activeCategories;
}

shared_ptr<Category> CategoryRepository::findById(int id) {
    auto categories = getCategories();
    auto categoryIt =
        find_if(categories.begin(), categories.end(),
                [id](const Category &category) { return category.id == id; });
    if (categoryIt != categories.end()) {
        return make_shared<Category>(*categoryIt);
    }
    return nullptr;
}