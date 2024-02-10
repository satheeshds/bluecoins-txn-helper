#include <vector>
#include <sstream>
#include <string>

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string join(const std::vector<std::string> &v, char delimiter)
{
    std::string s;
    for (size_t i = 0; i < v.size(); ++i)
    {
        s += v[i];
        if (i != v.size() - 1)
        {
            s += delimiter;
        }
    }
    return s;
}