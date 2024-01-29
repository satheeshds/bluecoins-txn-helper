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

std::vector<Item> db::getItems(std::string prefix) {
    std::string queryStr = "select distinct it.itemName, cct.childCategoryName, pct.parentCategoryName, lt.labelName \
                            from itemtable it inner join transactionstable tt on tt.itemID = it.itemTableID \
                            inner join childcategorytable cct on tt.categoryID = cct.categoryTableID \
                            inner join parentcategorytable pct on cct.parentCategoryID = pct.parentCategoryTableID \
                            inner join labelstable lt on tt.transactionstableid = lt.transactionidlabels \
                            where it.itemName like ?;";
    SQLite::Statement query(*database, queryStr);
    query.bind(1, prefix + "%");

    std::vector<Item> items;
    while (query.executeStep())
    {
        Item item;
        item.name = query.getColumn(0).getString();
        item.category = query.getColumn(1).getString();
        item.parent_category = query.getColumn(2).getString();
        item.label = query.getColumn(3).getString();
        items.push_back(item);
    }

    return items;
}