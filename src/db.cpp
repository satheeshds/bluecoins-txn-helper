#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <algorithm>
#include <cctype>
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

std::vector<Item> db::getItems(std::string prefix)
{
    std::string queryStr = "select distinct it.itemName, cct.childCategoryName, pct.parentCategoryName, \
                            (SELECT GROUP_CONCAT(lt.labelname) FROM labelstable lt WHERE lt.transactionidlabels = tt.transactionstableid) as labels \
                            from itemtable it inner join transactionstable tt on tt.itemID = it.itemTableID \
                            inner join childcategorytable cct on tt.categoryID = cct.categoryTableID \
                            inner join parentcategorytable pct on cct.parentCategoryID = pct.parentCategoryTableID \
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
        item.label.erase(std::remove_if(item.label.begin(), item.label.end(), ::isspace), item.label.end());
        items.push_back(item);
    }

    return items;
}

std::vector<Account> db::getAccounts(std::string prefix)
{
    SQLite::Statement query(*database, "select at.accountName, att.accountTypeName \
                                        from accountstable at inner join accounttypetable att on at.accounttypeid = att.accounttypetableid \
                                        where at.accountName like ?;");

    query.bind(1, "%" + prefix + "%");
    std::vector<Account> accounts;
    while (query.executeStep())
    {
        Account account;
        account.name = query.getColumn(0).getString();
        account.type = query.getColumn(1).getString();
        accounts.push_back(account);
    }

    return accounts;
}