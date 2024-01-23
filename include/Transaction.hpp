
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <string>
enum class TransactionType
{
    DEBIT,
    CREDIT
};

struct Transaction
{
    std::string date;
    std::string description;
    double amount;
    std::string account;
    TransactionType type;
};
#endif // TRANSACTION_HPP