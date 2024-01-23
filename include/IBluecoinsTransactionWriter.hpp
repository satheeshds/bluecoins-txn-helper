#ifndef BLUECOINSTRANSACTIONWRITER_HPP
#define BLUECOINSTRANSACTIONWRITER_HPP

#include <string>
#include <vector>
#include "BluecoinsTransaction.hpp"

class IBluecoinsTransactionWriter
{
public:
    virtual ~IBluecoinsTransactionWriter() = default;

    // Define your methods here. For example:
    virtual void open(const std::string &filename) = 0;
    virtual void close() = 0;
    virtual void writeTransactions(const std::vector<BluecoinsTransaction> &transactions) = 0;
};

#endif // BLUECOINSTRANSACTIONWRITER_HPP