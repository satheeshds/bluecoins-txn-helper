#ifndef ITRANSACTIONTRANSFORMER_HPP
#define ITRANSACTIONTRANSFORMER_HPP

#include "BluecoinsTransaction.hpp"
#include "Transaction.hpp"

class ITransactionTransformer
{
public:
    virtual ~ITransactionTransformer() = default;

    virtual std::vector<BluecoinsTransaction> transform(const std::vector<Transaction> &transaction) = 0;
};

#endif // ITRANSACTIONTRANSFORMER_HPP