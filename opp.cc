#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

bool isValidPhase1(const std::string &line) {
    boost::regex pattern(R"(\s*\u{3}\s\d+(,\d{1,3})?\s*)");
    return boost::regex_match(line, pattern);
}

bool isValidPhase2(const std::string &line) {
    boost::regex pattern(R"(\s*.*\s\d+(,\d{1,3})?\s\u{3}\s*)");
    return boost::regex_match(line, pattern);
}

bool isValidPhase3Header(const std::string &line) {
    boost::regex pattern(R"(\s*\d+\s\d+\s*)");
    return boost::regex_match(line, pattern);
}

bool isValidPhase3(const std::string &line) {
    boost::regex pattern(R"(\s*\d+(,\d{1,3})?\s\d+(,\d{1,3})?\s*)");
    return boost::regex_match(line, pattern);
}

void check(const std::string &line, std::function<bool(const std::string&)> foo) {
    if(foo(line)) {
        std::cout << line << " is valid\n";
    }
    else {
        std::cout << line << " is !!invalid\n";
    }
}

int main() {
    std::cout << "===First Phase:===\n\n";
    check("  PLN 11", isValidPhase1);
    check("PLN  55", isValidPhase1);
    check("\t  PLN  66  \t ", isValidPhase1);
    check("\t  PLN 55  \t ", isValidPhase1);
    check("PLN 1", isValidPhase1);
    check("USD 3,96", isValidPhase1);
    check("EUR 4,19", isValidPhase1);
    check("EUR 5,11", isValidPhase1);
    check("PHP 1,5", isValidPhase1);
    check("XAU 0", isValidPhase1);

    std::cout << "\n===Second Phase:===\n\n";
    check("  PLN 11", isValidPhase2);
    check("PLN  55", isValidPhase2);
    check("\t  PLN  66  \t ", isValidPhase2);
    check("\t  PLN 55  \t ", isValidPhase2);
    check("PLN 1", isValidPhase2);
    check("USD 3,96", isValidPhase2);
    check("EUR 4,19", isValidPhase2);
    check("EUR 5,11", isValidPhase2);
    check("PHP 1,5", isValidPhase2);
    check("XAU 0", isValidPhase2);
    check("Ala D 5,01 PLN", isValidPhase2);
    check("Ala  B 5 PLN", isValidPhase2);
    check("Ala A 5,00 PLN", isValidPhase2);
    check("Ala C 4,99 PLN", isValidPhase2);
    check("Baba 30 WWW", isValidPhase2);
    check("C         PLN", isValidPhase2);
    check("Miś 0,01 PLN", isValidPhase2);
    check("X 1,001 PHP", isValidPhase2);
    check("Y 1,003 PHP", isValidPhase2);
    check("1 PLN", isValidPhase2);

    std::cout << "\n===Third Phase Header:===\n\n";
    check("5 5", isValidPhase3Header);
    check("0 10", isValidPhase3Header);
    check("0,001 0,002", isValidPhase3Header);
    check("1,502 1,502", isValidPhase3Header);
    check("1,504 1,504", isValidPhase3Header);
    check("7 6", isValidPhase3Header);

    std::cout << "\n===Third Phase:===\n\n";
    check("5 5", isValidPhase3);
    check("0 10", isValidPhase3);
    check("0,001 0,002", isValidPhase3);
    check("1,502 1,502", isValidPhase3);
    check("1,504 1,504", isValidPhase3);
    check("7 6", isValidPhase3);
}
