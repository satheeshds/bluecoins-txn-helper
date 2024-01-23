#include "../include/ITransactionTransformer.hpp"
#include <vector>

class BluecoinsTransformer : public ITransactionTransformer
{
public:
    BluecoinsTransformer() = default;
    ~BluecoinsTransformer() = default;

    virtual std::vector<BluecoinsTransaction> transform(const std::vector<Transaction> &transactions) override
    {
        std::vector<BluecoinsTransaction> bluecoins_transactions;

        for (const auto &t : transactions)
        {
            BluecoinsTransaction bluecoins_transaction;
            bluecoins_transaction.type = t.type == TransactionType::DEBIT ? 'e' : 'i';
            bluecoins_transaction.date = t.date;
            bluecoins_transaction.item_or_payee = t.description;
            bluecoins_transaction.amount = t.amount;
            bluecoins_transaction.parent_category = "";
            bluecoins_transaction.category = "";
            bluecoins_transaction.account_type = "";
            bluecoins_transaction.account = t.account;
            bluecoins_transaction.notes = "";
            bluecoins_transaction.labels = "";
            bluecoins_transaction.status = "";
            bluecoins_transaction.split = "";

            std::cout << bluecoins_transaction << std::endl;

            bluecoins_transactions.push_back(bluecoins_transaction);
        }

        return bluecoins_transactions;
    }
};

std::istream &operator>>(std::istream &is, BluecoinsTransaction &transaction)
{
    is >> transaction.item_or_payee;
    // Add more fields if needed
    return is;
}

std::ostream &operator<<(std::ostream &os, const BluecoinsTransaction &transaction)
{
    os << "Type: " << transaction.type << "\n"
       << "Date: " << transaction.date << "\n"
       << "Item or Payee: " << transaction.item_or_payee << "\n"
       << "Amount: " << transaction.amount << "\n"
       << "Account: " << transaction.account << "\n";
    // Add more fields if needed
    return os;
}