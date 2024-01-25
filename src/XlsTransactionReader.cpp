// Assuming IXlsTransactionReader is defined in this header
#include "../include/IXlsTransactionReader.hpp"
#include <xls.h>
#include <stdexcept>
#include <iostream>
#include "globals.h"
using namespace xls;

class XlsTransactionReader : public IXlsTransactionReader
{
private:
    xls_error_t error = LIBXLS_OK;
    xls::xlsWorkBook *wb = NULL;
    std::string filename;
    tm convertToDate(double d)
    {
        int wholeDays = static_cast<int>(d);
        double fractionalDay = d - wholeDays;
        int secondsInDay = static_cast<int>(24 * 60 * 60 * fractionalDay + 0.5);
        // Convert Excel date to tm struct
        struct tm date = {0};
        date.tm_hour = secondsInDay / 3600;
        date.tm_min = (secondsInDay % 3600) / 60;
        date.tm_sec = secondsInDay % 60;
        date.tm_mday = wholeDays - 25568;
        date.tm_year = 70;
        mktime(&date); // Normalize the tm struct
        if (isVerbose)
            printf("      Date: %04d-%02d-%02d %02d:%02d:%02d\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
        return date;
    }

public:
    XlsTransactionReader(const std::string &fname)
    {
        filename = fname; // Implement this method
    }
    void open() override
    {
        // Implement this method
        // debug settings
        // xls::xls(10);
        wb = xls_open_file(filename.c_str(), "UTF-8", &error);
        if (wb == NULL)
        {
            printf("Error reading file: %s\n", xls_getError(error));
            exit(1);
        }
    }

    void close() override
    {
        if (wb != NULL)
        {
            xls_close_WB(wb);
        }
        // Implement this method
    }

    std::vector<Transaction> getTransactions() override
    {
        if (wb == NULL)
        {
            throw std::runtime_error("Workbook is not opened");
        }

        if (isVerbose)
            std::cout << "Number of sheets: " << wb->sheets.count << std::endl;
        std::vector<Transaction> transactions;

        for (int i = 0; i < wb->sheets.count; i++)
        {
            xlsWorkSheet *ws = xls_getWorkSheet(wb, i);
            error = xls_parseWorkSheet(ws);
            if (ws == NULL)
            {
                printf("Error reading sheet: %s\n", xls_getError(error));
                exit(1);
            }

            if (isVerbose)
                printf("Sheet: %s\n", ws->workbook->sheets.sheet[i].name);
            if (isVerbose)
                printf("  Max col: %d\n", ws->rows.lastcol);
            if (isVerbose)
                printf("  Max row: %d\n", ws->rows.lastrow);

            // transaction header starts from row 19
            for (int row = 20; row <= ws->rows.lastrow - 2; row++)
            {
                Transaction transaction;
                transaction.account = "SBI Technopark";
                if (isVerbose)
                    printf("  Row %d\n", row);
                xlsRow *rowP = xls_row(ws, row);
                for (int col = 0; col <= ws->rows.lastcol; col++)
                {
                    if (isVerbose)
                        printf("    Col %d\n", col);
                    xlsCell *cell = &rowP->cells.cell[col];
                    if (cell == NULL)
                    {
                        if (isVerbose)
                            printf("Error reading cell: %s\n", xls_getError(error));
                        exit(1);
                    }

                    if (isVerbose)
                        printf("      %s\n", cell->str);

                    switch (col)
                    {
                    case DATE_COLUMN:
                        transaction.date = this->convertToDate(cell->d);
                        break;
                    case DESCRIPTION_COLUMN:
                        transaction.description = cell->str;
                        break;
                    case DEBIT_COLUMN:
                    case CREDIT_COLUMN:
                        if (cell->d > 0)
                        {
                            transaction.amount = cell->d;
                            transaction.type = col == DEBIT_COLUMN ? TransactionType::DEBIT : TransactionType::CREDIT;
                        }

                        break;

                    default:
                        break;
                    }
                }
                transactions.push_back(transaction);
            }
        }

        // Implement this method
        return transactions;
    }
};