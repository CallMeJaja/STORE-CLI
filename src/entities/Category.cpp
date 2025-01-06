#include "entities/Category.hpp"

Category::Category(int id, string name, bool isActive, bool isDefault)
    : id(id), name(name), isActive(isActive), isDefault(isDefault) {}