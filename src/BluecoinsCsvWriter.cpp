#include "../include/IBluecoinsTransactionWriter.hpp"
#include <fstream>

class BluecoinsCsvWriter : public IBluecoinsTransactionWriter
{
private:
    std::ofstream m_file;

public:
    BluecoinsCsvWriter() = default;
    ~BluecoinsCsvWriter() = default;

    virtual void open(const std::string &filename) override
    {
        m_file.open(filename);
    }

    virtual void close() override
    {
        m_file.close();
    }

    virtual void writeTransactions(const std::vector<BluecoinsTransaction> &transactions) override
    {
        m_file << "(1)Type,(2)Date,(3)Item or Payee,(4)Amount,(5)Parent Category,";
        m_file << "(6)Category,(7)Account Type,(8)Account,(9)Notes,(10) Label,(11) Status,(12) Split" << std::endl;
        for (const auto &t : transactions)
        {
            m_file << t << std::endl;
        }
    }
};

std::string quoteIfNeeded(const std::string &str)
{
    if (str.find(',') != std::string::npos || str.find(' ') != std::string::npos)
    {
        return "\"" + str + "\"";
    }
    return str;
}

std::ostream &operator<<(std::ostream &os, const BluecoinsTransaction &transaction)
{
    char dateStr[100];
    strftime(dateStr, sizeof(dateStr), "%m/%d/%Y", &transaction.date);
    os << transaction.type << ","
       << dateStr << ","
       << quoteIfNeeded(transaction.item_or_payee) << ","
       << transaction.amount << ","
       << quoteIfNeeded(transaction.parent_category) << ","
       << quoteIfNeeded(transaction.category) << ","
       << quoteIfNeeded(transaction.account_type) << ","
       << transaction.account << ","
       << quoteIfNeeded(transaction.notes) << ","
       << quoteIfNeeded(transaction.labels) << ","
       << transaction.status << ","
       << transaction.split << std::endl;
    // Add more fields if needed
    return os;
}