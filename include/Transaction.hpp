
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <string>

#define DATE_COLUMN 0
#define DESCRIPTION_COLUMN 2
#define DEBIT_COLUMN 4
#define CREDIT_COLUMN 5

enum class TransactionType
{
    DEBIT,
    CREDIT
};

struct Transaction
{
    tm date;
    std::string description;
    double amount;
    std::string account;
    TransactionType type;
};
#endif // TRANSACTION_HPP