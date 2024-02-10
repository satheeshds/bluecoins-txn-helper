#ifndef HELPER_HPP
#define HELPER_HPP

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, char delimiter);
std::string join(const std::vector<std::string> &v, char delimiter);
#endif // DB_HPP