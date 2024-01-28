#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>
#include "../include/db.hpp"

db::db(const std::string &dbPath)
{
    this->database = new SQLite::Database(dbPath, SQLite::OPEN_READONLY);
}

db::~db()
{
}

std::vector<Category> db::getCategories()
{
    // SQLite::Database db(dbPath, SQLite::OPEN_READONLY);
    SQLite::Statement query(*database, "select categorytableId, childCategoryName, parentCategoryName from childcategorytable cct inner join parentcategorytable pct on cct.parentCategoryID = pct.parentCategoryTableID;");

    std::vector<Category> categories;
    while (query.executeStep())
    {
        Category category;
        category.id = query.getColumn(0).getInt();
        category.name = query.getColumn(1).getString();
        category.parent_category = query.getColumn(2).getString();
        categories.push_back(category);
    }

    return categories;
}