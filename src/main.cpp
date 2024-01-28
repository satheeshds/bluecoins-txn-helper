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
    std::string database_file;
    // std::string output_file;
    app.add_option("-f,--file", xls_file, "Bank statement in xlsx format")->check(CLI::ExistingFile)->required();
    app.add_option("-d,--database", database_file, "Output file name");
    // app.add_option("-o,--output", output_file, "Output file name");

    app.add_flag("-v,--verbose", isVerbose, "Print verbose information");

    CLI11_PARSE(app, argc, argv);

    if (isVerbose)
        std::cout << "file = " << xls_file << std::endl;

    if (isVerbose)
        std::cout << "reading xls" << std::endl;
    IXlsTransactionReader *reader = new XlsTransactionReader(xls_file);
    reader->open();
    auto transactions = reader->getTransactions();
    reader->close();

    if (isVerbose)
        std::cout << "transforming transactions" << std::endl;
    ITransactionTransformer *transformer = new BluecoinsTransformer(database_file);
    auto bluecoins_transactions = transformer->transform(transactions);

    if (isVerbose)
        std::cout << "writing transactions" << std::endl;
    IBluecoinsTransactionWriter *writer = new BluecoinsCsvWriter();
    writer->open("import.csv");
    writer->writeTransactions(bluecoins_transactions);
    writer->close();

    return 0;
}