#include "../include/IXlsTransactionReader.hpp"
#include <fstream>
#include <sstream>
#include <deque>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "globals.h"

class FileTransactionReader : public IXlsTransactionReader
{
private:
    std::string filename;
    tm convertToDate(const std::string &d)
    {
        struct tm date = {0};
        std::istringstream iss(d);
        iss >> std::get_time(&date, "%d %b %Y");

        if (iss.fail())
            throw std::runtime_error("Failed to parse date string " + d + " to tm struct ");

        return date;
    }

public:
    FileTransactionReader(const std::string &fname)
    {
        filename = fname;
    }
    void open() override
    {
    }

    void close() override
    {
        // Implement this method
    }

    std::vector<Transaction> getTransactions() override
    {
        // Implement this method
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string line;
            int line_count = 0;
            std::vector<Transaction> transactions;
            std::deque<std::string> last_three_lines;
            while (std::getline(file, line))
            {
                line_count++;
                if (line_count <= 20)
                    continue; // skip first line (header

                last_three_lines.push_back(line);
                if (last_three_lines.size() > 3)
                {
                    std::istringstream iss(last_three_lines.front());
                    std::string value;
                    // std::cout << "|";
                    int column_count = 0;
                    Transaction t;
                    while (std::getline(iss, value, '\t'))
                    {
                        switch (column_count++)
                        {
                        case DATE_COLUMN:
                            t.date = convertToDate(value);
                            break;
                        case DESCRIPTION_COLUMN:
                            t.description = value;
                            break;
                        case DEBIT_COLUMN:
                        case CREDIT_COLUMN:
                            if (value != " ")
                            {
                                value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                                t.amount = std::stod(value);
                                t.type = column_count == DEBIT_COLUMN ? TransactionType::DEBIT : TransactionType::CREDIT;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    transactions.push_back(t);
                    last_three_lines.pop_front();
                }
            }

            file.close();
            return transactions;
        }
        else
        {
            throw std::runtime_error("Failed to open file " + filename + " for reading");
        }
    }
};