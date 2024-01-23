#ifndef XLSREADER_HPP
#define XLSREADER_HPP

#include <string>
#include <vector>
#include "Transaction.hpp"

class IXlsTransactionReader
{
public:
    IXlsTransactionReader(const std::string &filename);
    IXlsTransactionReader() = default;
    virtual ~IXlsTransactionReader() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual std::vector<Transaction> getTransactions() = 0;
};

#endif // XLSREADER_HPP