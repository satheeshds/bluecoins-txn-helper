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

class db
{
private:
    SQLite::Database *database;

public:
    db(const std::string &dbPath);
    ~db();
    std::vector<Category> getCategories();
};

#endif // DB_HPP