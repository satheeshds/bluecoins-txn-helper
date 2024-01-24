#include <CLI/CLI.hpp>
#include "../include/IXlsTransactionReader.hpp"
#include "../include/ITransactionTransformer.hpp"
#include "../include/IBluecoinsTransactionWriter.hpp"
#include "XlsTransactionReader.cpp"
#include "BluecoinsTransformer.cpp"
#include "BluecoinsCsvWriter.cpp"
#include "../include/globals.h"

int main(int argc, char **argv)
{
    CLI::App app{"Convert bank statement to bluecoins transaction file"};

    std::string xls_file;
    app.add_option("-f,--file", xls_file, "Bank statement in xlsx format")->check(CLI::ExistingFile)->required();

    app.add_flag("-v,--verbose", isVerbose, "Print verbose information");

    CLI11_PARSE(app, argc, argv);

    if(isVerbose) std::cout << "file = " << xls_file << std::endl;

    if(isVerbose) std::cout << "reading xls" << std::endl;
    IXlsTransactionReader *reader = new XlsTransactionReader(xls_file);
    reader->open();
    auto transactions = reader->getTransactions();
    reader->close();

    if(isVerbose) std::cout << "transforming transactions" << std::endl;
    ITransactionTransformer *transformer = new BluecoinsTransformer();
    auto bluecoins_transactions = transformer->transform(transactions);

    if(isVerbose) std::cout << "writing transactions" << std::endl;
    IBluecoinsTransactionWriter *writer = new BluecoinsCsvWriter();
    writer->open("bluecoins.csv");
    writer->writeTransactions(bluecoins_transactions);
    writer->close();

    return 0;
}