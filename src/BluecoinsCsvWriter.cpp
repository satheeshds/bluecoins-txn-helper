#include "../include/IBluecoinsTransactionWriter.hpp"

class BluecoinsCsvWriter : public IBluecoinsTransactionWriter
{
public:
    BluecoinsCsvWriter() = default;
    ~BluecoinsCsvWriter() = default;

    virtual void open(const std::string &filename) override
    {
    }

    virtual void close() override
    {
    }

    virtual void writeTransactions(const std::vector<BluecoinsTransaction> &transactions) override
    {
    }
};