#ifndef DB_HPP
#define DB_HPP

#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

struct IStringable
{
    virtual std::string toString() const = 0; // Pure virtual function
};

struct Category
{
    int id;
    std::string name;
    std::string parent_category;
};

struct Account : public IStringable
{
    std::string name;
    std::string type;
    std::string toString() const override
    {
        return name + "|" + type;
    }
};

struct Item : public IStringable
{
    std::string name;
    std::string category;
    std::string parent_category;
    std::string label;
    std::string toString() const override
    {
        return name + "|" + category + "|" + parent_category + "|" + label;
    }
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
    std::vector<Account> getAccounts(std::string prefix = "");
};

#endif // DB_HPP