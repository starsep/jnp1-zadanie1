#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>


int currentLineNumber;
std::map<std::string, long double> currencies; //dla kodu waluty przelicznik

void reportError(const std::string &line) {
    std::cerr << "Error in line " << currentLineNumber << ':' << line << '\n';
}

//tworzy liczbę z części sprzed przecinka i po
//np. makeNumber("3", "14") == 3.14
//    makeNumber("42", "") == 42.0
long double makeNumber(const std::string &a, const std::string &b) {
    std::stringstream sstream;
    sstream << a;
    if (b.size() > 0) {
        sstream << '.' << b;
    }
    return std::stold(sstream.str());
}

bool checkPhase1(const std::string &line) {
    boost::regex pattern(R"(\s*(\u{3})\s((\d+)(,(\d{1,3}))?)\s*)");
    bool matched = boost::regex_match(line, pattern);
    if (matched) {
        boost::smatch result;
        boost::regex_search(line, result, pattern);
        std::string currencyCode = result[1];
        long double currencyValue = makeNumber(result[3], result[5]);
        std::cerr << currencyCode << " = " << currencyValue << '\n';
        if (currencyValue <= 0.0 || currencies.find(currencyCode) != currencies.end()) {
            reportError(line);
        }
        else {
            currencies[currencyCode] = currencyValue;
        }
    }

    return matched;
}

bool checkPhase2(const std::string &line) {
    boost::regex pattern(R"(\s*.*\s\d+(,\d{1,3})?\s\u{3}\s*)");
    bool matched = boost::regex_match(line, pattern);
    return matched;
}

bool checkPhase3(const std::string &line) {
    boost::regex pattern(R"(\s*\d+(,\d{1,3})?\s\d+(,\d{1,3})?\s*)");
    bool matched = boost::regex_match(line, pattern);
    return matched;
}


void solve() {
    std::array<std::function<bool(const std::string &)>,3> phases
            {{checkPhase1, checkPhase2, checkPhase3}};
    std::string line;
    size_t currentPhase = 0;
    for (currentLineNumber = 1; std::getline(std::cin, line); currentLineNumber++) {
        bool success = false;
        for (size_t i = currentPhase; i < phases.size() && !success; i++) {
            if (phases[i](line)) {
                currentPhase = i;
                success = true;
            }
        }
        if (!success) {
            reportError(line);
        }
        std::cerr << line << "++++++++++" << currentPhase << '\n';
    }
}

void check(const std::string &line, std::function<bool(const std::string &)> foo) {
    if (foo(line)) {
        std::cout << line << " is valid\n";
    }
    else {
        std::cout << line << " is !!invalid\n";
    }
}

void tests() {
    std::cout << "===First Phase:===\n\n";
    check("  PLN 11", checkPhase1);
    check("PLN  55", checkPhase1);
    check("\t  PLN  66  \t ", checkPhase1);
    check("\t  PLN 55  \t ", checkPhase1);
    check("PLN 1", checkPhase1);
    check("USD 3,96", checkPhase1);
    check("EUR 4,19", checkPhase1);
    check("EUR 5,11", checkPhase1);
    check("PHP 1,5", checkPhase1);
    check("XAU 0", checkPhase1);

    std::cout << "\n===Second Phase:===\n\n";
    check("  PLN 11", checkPhase2);
    check("PLN  55", checkPhase2);
    check("\t  PLN  66  \t ", checkPhase2);
    check("\t  PLN 55  \t ", checkPhase2);
    check("PLN 1", checkPhase2);
    check("USD 3,96", checkPhase2);
    check("EUR 4,19", checkPhase2);
    check("EUR 5,11", checkPhase2);
    check("PHP 1,5", checkPhase2);
    check("XAU 0", checkPhase2);
    check("Ala D 5,01 PLN", checkPhase2);
    check("Ala  B 5 PLN", checkPhase2);
    check("Ala A 5,00 PLN", checkPhase2);
    check("Ala C 4,99 PLN", checkPhase2);
    check("Baba 30 WWW", checkPhase2);
    check("C         PLN", checkPhase2);
    check("Miś 0,01 PLN", checkPhase2);
    check("X 1,001 PHP", checkPhase2);
    check("Y 1,003 PHP", checkPhase2);
    check("1 PLN", checkPhase2);

    std::cout << "\n===Third Phase:===\n\n";
    check("5 5", checkPhase3);
    check("0 10", checkPhase3);
    check("0,001 0,002", checkPhase3);
    check("1,502 1,502", checkPhase3);
    check("1,504 1,504", checkPhase3);
    check("7 6", checkPhase3);
}

int main() {
    solve();
}
