
#ifndef BlUECOINSTRANSACTION_HPP
#define BlUECOINSTRANSACTION_HPP
#include <string>
struct BluecoinsTransaction
{
    char type;
    std::string date;
    std::string item_or_payee;
    double amount;
    std::string parent_category;
    std::string category;
    std::string account_type;
    std::string account;
    std::string notes;
    std::string labels;
    std::string status;
    std::string split;
    /* data */
};

std::ostream &operator<<(std::ostream &os, const BluecoinsTransaction &transaction);
std::istream &operator>>(std::istream &is, BluecoinsTransaction &transaction);
#endif // BlUECOINSTRANSACTION_HPP
