#include "../include/ITransactionTransformer.hpp"
#include "../include/db.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <functional>
#include <ncurses.h>

template <typename T>

T printAndHighlightChoices(const std::string title, std::function<std::vector<T>(std::string)> getItems, std::string input)
{
    initscr();
    noecho();
    cbreak();

    keypad(stdscr, TRUE);

    int choice;
    int highlight = 0;
    auto choices = getItems(input);
    while (1)
    {
        clear();
        printw("%s\n", title.c_str());
        printw("Existing details for: %s\n", input.c_str());
        for (int i = 0; i < choices.size(); i++)
        {
            if (i == highlight)
            {
                wattron(stdscr, A_REVERSE);
            }
            printw("%s\n", choices[i].toString().c_str());
            if (i == highlight)
            {
                wattroff(stdscr, A_REVERSE);
            }
        }

        // Move the cursor to the input
        move(1, strlen("Existing details for: ") + input.length());

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
            if (highlight == choices.size())
            {
                highlight = choices.size() - 1;
            }
            break;
        case 27: // Escape key
            endwin();
            return T{};
        case 10: // Enter key
            endwin();
            return choices[highlight];
        case KEY_BACKSPACE:
            if (input.empty())
                break;

            input.pop_back();
            if (input.empty())
                break;

            choices = getItems(input);
            highlight = 0;
            break;
        default:
            if (isprint(choice))
            {
                input += choice;
                choices = getItems(input);
                highlight = 0;
            }
            break;
        }
        refresh();
    }
}

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

    Account getSelectedAccount(std::string input)
    {
        return printAndHighlightChoices<Account>(
            "Select account", [this](std::string input)
            { return this->m_db->getAccounts(input); },
            input);
    }

    Item getSelectedItem(const std::string &description)
    {
        std::string input = cleanDescription(description);
        return printAndHighlightChoices<Item>(
            "Transaction description: " + description, [this](std::string input)
            { return this->m_db->getItems(input); },
            input);
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

            Item selectedItem = getSelectedItem(t.description);

            BluecoinsTransaction bluecoins_transaction;
            bluecoins_transaction.type = t.type == TransactionType::DEBIT ? 'e' : 'i';
            bluecoins_transaction.date = t.date;
            bluecoins_transaction.amount = t.amount;
            bluecoins_transaction.account_type = "Bank";
            bluecoins_transaction.account = t.account;
            bluecoins_transaction.notes = "";
            bluecoins_transaction.status = "";
            bluecoins_transaction.split = "";

            if (selectedItem.name == "")
            {
                bluecoins_transaction.item_or_payee = cleanDescription(t.description);
                std::cin >> bluecoins_transaction;
            }
            else
            {
                bluecoins_transaction.item_or_payee = selectedItem.name;
                bluecoins_transaction.parent_category = selectedItem.parent_category;
                bluecoins_transaction.category = selectedItem.category;
                bluecoins_transaction.labels = selectedItem.label;
            }

            if (selectedItem.parent_category == "(Transfer)")
            {
                BluecoinsTransaction transfer_transaction;
                transfer_transaction.type = 't';
                bluecoins_transaction.type = 't';
                transfer_transaction.date = t.date;
                transfer_transaction.amount = t.amount;
                transfer_transaction.item_or_payee = selectedItem.name;
                transfer_transaction.parent_category = selectedItem.parent_category;
                transfer_transaction.category = selectedItem.category;
                transfer_transaction.labels = selectedItem.label;
                auto selectedAccount = getSelectedAccount("");
                transfer_transaction.account_type = selectedAccount.type;
                transfer_transaction.account = selectedAccount.name;

                if (t.type == TransactionType::DEBIT)
                {
                    bluecoins_transaction.amount = -t.amount;
                    bluecoins_transactions.push_back(bluecoins_transaction);
                    bluecoins_transactions.push_back(transfer_transaction);
                }
                else
                {
                    transfer_transaction.amount = -t.amount;
                    bluecoins_transactions.push_back(transfer_transaction);
                    bluecoins_transactions.push_back(bluecoins_transaction);
                }
                continue;
            }

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
