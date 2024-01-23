// Assuming IXlsTransactionReader is defined in this header
#include "../include/IXlsTransactionReader.hpp"
#include <xls.h>
#include <stdexcept>
#include <iostream>
using namespace xls;

class XlsTransactionReader : public IXlsTransactionReader
{
private:
    xls_error_t error = LIBXLS_OK;
    xls::xlsWorkBook *wb = NULL;
    std::string filename;

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

            printf("Sheet: %s\n", ws->workbook->sheets.sheet[i].name);
            printf("  Max col: %d\n", ws->rows.lastcol);
            printf("  Max row: %d\n", ws->rows.lastrow);

            // transaction header starts from row 19
            for (int row = 20; row <= ws->rows.lastrow; row++)
            {
                Transaction transaction;
                printf("  Row %d\n", row);
                for (int col = 0; col <= ws->rows.lastcol; col++)
                {
                    printf("    Col %d\n", col);
                    xlsCell *cell = xls_cell(ws, row, col);
                    if (cell == NULL)
                    {
                        printf("Error reading cell: %s\n", xls_getError(error));
                        exit(1);
                    }

                    switch (cell->id)
                    {
                    case XLS_RECORD_BLANK:
                        /* code */
                        break;
                    case XLS_RECORD_NUMBER:
                        printf("      Number: %f\n", cell->d);
                        break;
                    case XLS_RECORD_STRING:
                        break;
                    default:
                        break;
                    }

                    if (cell->id == 0x27e)
                    {
                        printf("  Cell[%d][%d]: %s\n", row, col, cell->str);
                    }
                }
                transactions.push_back(transaction);
            }
        }

        // Implement this method
        return std::vector<Transaction>();
    }
};