#ifndef DB_HPP
#define DB_HPP

#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

struct Category
{
    int id;
    std::string name;
    std::string parent_category;
};

struct Item
{
    std::string name;
    std::string category;
    std::string parent_category;
    std::string label;
};

class db
{
private:
    SQLite::Database *database;

public:
    db(const std::string &dbPath);
    ~db();
    std::vector<Category> getCategories();
    std::vector<Item> getItems(std::string prefix);
};

#endif // DB_HPP