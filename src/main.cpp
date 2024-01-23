#include <CLI/CLI.hpp>
#include "../include/IXlsTransactionReader.hpp"
#include "../include/ITransactionTransformer.hpp"
#include "../include/IBluecoinsTransactionWriter.hpp"
#include "XlsTransactionReader.cpp"
#include "BluecoinsTransformer.cpp"
#include "BluecoinsCsvWriter.cpp"

int main(int argc, char **argv)
{
    CLI::App app{"Convert bank statement to bluecoins transaction file"};

    std::string xls_file;
    app.add_option("-f,--file", xls_file, "Bank statement in xlsx format")->check(CLI::ExistingFile)->required();

    CLI11_PARSE(app, argc, argv);

    std::cout << "file = " << xls_file << std::endl;

    std::cout << "reading xls" << std::endl;
    IXlsTransactionReader *reader = new XlsTransactionReader(xls_file);
    reader->open();
    auto transactions = reader->getTransactions();
    reader->close();

    std::cout << "transforming transactions" << std::endl;
    ITransactionTransformer *transformer = new BluecoinsTransformer();
    auto bluecoins_transactions = transformer->transform(transactions);

    std::cout << "writing transactions" << std::endl;
    IBluecoinsTransactionWriter *writer = new BluecoinsCsvWriter();
    writer->open("bluecoins.csv");
    writer->writeTransactions(bluecoins_transactions);
    writer->close();

    return 0;
}