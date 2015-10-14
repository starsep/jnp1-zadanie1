#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

bool isPhase1Valid(const std::string &line) {
    boost::regex pattern(R"(\s*\u{3}\s\d+(,\d{1,3})?\s*)");
    return boost::regex_match(line, pattern);
}

bool isPhase2Valid(const std::string &line) {
    boost::regex pattern(R"(\s*.*\s\d+(,\d{1,3})?\s\u{3}\s*)");
    return boost::regex_match(line, pattern);
}

bool isISO4217valid(const std::string &code) {
    boost::regex pattern("[A-Z]{3}");
    return boost::regex_match(code, pattern);
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
    check("  PLN 11", isPhase1Valid);
    check("PLN  55", isPhase1Valid);
    check("\t  PLN  66  \t ", isPhase1Valid);
    check("\t  PLN 55  \t ", isPhase1Valid);
    check("PLN 1", isPhase1Valid);
    check("USD 3,96", isPhase1Valid);
    check("EUR 4,19", isPhase1Valid);
    check("EUR 5,11", isPhase1Valid);
    check("PHP 1,5", isPhase1Valid);
    check("XAU 0", isPhase1Valid);

    std::cout << "\n===Second Phase:===\n\n";
    check("  PLN 11", isPhase2Valid);
    check("PLN  55", isPhase2Valid);
    check("\t  PLN  66  \t ", isPhase2Valid);
    check("\t  PLN 55  \t ", isPhase2Valid);
    check("PLN 1", isPhase2Valid);
    check("USD 3,96", isPhase2Valid);
    check("EUR 4,19", isPhase2Valid);
    check("EUR 5,11", isPhase2Valid);
    check("PHP 1,5", isPhase2Valid);
    check("XAU 0", isPhase2Valid);
    check("Ala D 5,01 PLN", isPhase2Valid);
    check("Ala  B 5 PLN", isPhase2Valid);
    check("Ala A 5,00 PLN", isPhase2Valid);
    check("Ala C 4,99 PLN", isPhase2Valid);
    check("Baba 30 WWW", isPhase2Valid);
    check("C         PLN", isPhase2Valid);
    check("Miś 0,01 PLN", isPhase2Valid);
    check("X 1,001 PHP", isPhase2Valid);
    check("Y 1,003 PHP", isPhase2Valid);
    check("1 PLN", isPhase2Valid);
}
