#include "../include/ITransactionTransformer.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include "../include/db.hpp"
#include <ncurses.h>

class BluecoinsTransformer : public ITransactionTransformer
{
private:
    std::string cleanDescription(const std::string &description)
    {
        std::vector<std::string> tokens = split(description, '/');
        std::string lastWord = tokens[tokens.size() - 1];
        std::size_t found = lastWord.rfind("--");
        if (found != std::string::npos)
        {
            lastWord.erase(found, 2);
        }
        return lastWord;
    }

    Item getSelectedItem(const std::string &description)
    {
        auto items = this->m_db->getItems(description);
        items.push_back(Item{"Exit", "None", "None", "None"});
        initscr();
        noecho();
        cbreak();

        keypad(stdscr, TRUE);
        printw("Select an item:\n");
        printw("Use arrow keys to go up and down, press enter to select an item\n");
        printw("Item\t| Category\t| Parent Category\t| Label\n");
        int choice;
        int highlight = 0;
        while (1)
        {
            clear();
            for (int i = 0; i < items.size(); i++)
            {
                if (i == highlight)
                {
                    wattron(stdscr, A_REVERSE);
                }
                printw("%s\t| %s\t| %s\t| %s\n ", items[i].name.c_str(), items[i].category.c_str(), items[i].parent_category.c_str(), items[i].label.c_str());
                if (i == highlight)
                {
                    wattroff(stdscr, A_REVERSE);
                }
            }

            // Wait for the user to press a key
            choice = getch();

            switch (choice)
            {
            case KEY_UP:
                highlight--;
                if (highlight == -1)
                {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == items.size())
                {
                    highlight = items.size() - 1;
                }
                break;
            case 10:
                endwin();
                return items[highlight];
                break;
            }
            /* code */
        }
        refresh();

        // End ncurses mode
        // endwin();
    }

    db *m_db;
    std::vector<Category> categories;

public:
    BluecoinsTransformer(const std::string &dbPath)
    {
        if (!dbPath.empty() && std::filesystem::exists(dbPath))
        {
            this->m_db = new db(dbPath);
            categories = this->m_db->getCategories();
        }
    };
    ~BluecoinsTransformer() = default;

    virtual std::vector<BluecoinsTransaction> transform(const std::vector<Transaction> &transactions) override
    {
        std::vector<BluecoinsTransaction> bluecoins_transactions;

        for (const auto &t : transactions)
        {
            std::cout << "-----------------------------" << std::endl;
            std::cout << t << std::endl;
            std::cout << "Convert to BluecoinsTransaction? (y/n): ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "n")
            {
                continue;
            }

            // for (const auto &c : categories)
            // {
            //     std::cout << "Category: " << c.name << std::endl;
            //     std::cout << "Parent Category: " << c.parent_category << std::endl;
            // }

            Item selectedItem = getSelectedItem(cleanDescription(t.description));
            if(selectedItem.name == "Exit") {
                selectedItem = Item{cleanDescription(t.description), "", "", ""};
            }

            BluecoinsTransaction bluecoins_transaction;
            bluecoins_transaction.type = t.type == TransactionType::DEBIT ? 'e' : 'i';
            bluecoins_transaction.date = t.date;
            bluecoins_transaction.item_or_payee = selectedItem.name;
            bluecoins_transaction.amount = t.amount;
            bluecoins_transaction.parent_category = selectedItem.parent_category;
            bluecoins_transaction.category = selectedItem.category;
            bluecoins_transaction.account_type = "Bank";
            bluecoins_transaction.account = t.account;
            bluecoins_transaction.notes = "";
            bluecoins_transaction.labels = selectedItem.label;
            bluecoins_transaction.status = "";
            bluecoins_transaction.split = "";

            std::cin >> bluecoins_transaction;

            bluecoins_transactions.push_back(bluecoins_transaction);
        }

        return bluecoins_transactions;
    }
};

std::istream &operator>>(std::istream &is, BluecoinsTransaction &transaction)
{
    std::cout << " type : " << transaction.type << std::endl;

    char dateStr[100];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", &transaction.date);
    std::cout << " date : " << dateStr << std::endl;

    std::cout << "Enter item or payee (default: " << transaction.item_or_payee << "): ";
    std::string input;
    std::getline(is, input);
    if (!input.empty())
    {
        transaction.item_or_payee = input;
    }

    std::cout << "Enter category (default: " << transaction.category << "): ";
    std::getline(is, input);
    if (!input.empty())
    {
        transaction.category = input;
    }

    std::cout << "Enter parent category (default:" << transaction.parent_category << "): ";
    std::getline(is, input);
    if (!input.empty())
    {
        transaction.parent_category = input;
    }

    std::cout << "Amount :" << transaction.amount << std::endl;
    std::cout << "Account :" << transaction.account << std::endl;
    std::cout << "Enter label (default: " << transaction.labels << "): ";
    std::getline(is, input);
    if (!input.empty())
    {
        transaction.labels = input;
    }

    // Add more fields if needed
    return is;
}

std::ostream &operator<<(std::ostream &os, const Transaction &transaction)
{
    char dateStr[100];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", &transaction.date);
    os << "Date: " << dateStr << "\n"
       << "Description: " << transaction.description << "\n"
       << "Amount: " << transaction.amount << "\n";
    return os;
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
