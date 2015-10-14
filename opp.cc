#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

bool isPhase1Valid(const std::string &line) {
    boost::regex pattern(R"(\s*[A-Z]{3}\s\d+(,\d{1,3})?\s*)");
    return boost::regex_match(line, pattern);
}

bool isISO4217valid(const std::string &code) {
    boost::regex pattern("[A-Z]{3}");
    return boost::regex_match(code, pattern);
}

void check(const std::string &line) {
    if(isPhase1Valid(line)) {
        std::cout << line << " is valid\n";
    }
    else {
        std::cout << line << " is !!invalid\n";
    }
}

int main() {
    check("  PLN 11");
    check("PLN  55");
    check("\t  PLN  66  \t ");
    check("\t  PLN 55  \t ");
    check("PLN 1");
    check("USD 3,96");
    check("EUR 4,19");
    check("EUR 5,11");
    check("PHP 1,5");
    check("XAU 0");
}
