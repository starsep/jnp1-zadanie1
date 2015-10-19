#include <boost/regex.hpp>
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <iomanip>
#include <exception>

typedef std::tuple<std::string, long double, std::string, int> contribution;
typedef std::vector<contribution>::iterator conIterator;

enum {
    name = 0,
    amount = 1,
    currency = 2,
    number = 3
};

bool isSorted = false;
bool error = false;
int currentLineNumber;
std::map<std::string, long double> currencies; //dla kodu waluty przelicznik
std::vector <contribution> contributions;

void reportError(const std::string &line) {
    if (!error) {
        std::cerr << "Error in line " << currentLineNumber << ':' << line << '\n';
        error = true;
    }
}

//ZAŁOŻENIE: zera wiodące to błąd
//tworzy liczbę z części sprzed przecinka i po
//np. makeNumber("3", "14") == 3.14
//    makeNumber("42", "") == 42.0
bool makeNumber(const std::string &a, const std::string &b, long double &result) {
    std::stringstream sstream;
    sstream << a;
    if (a[0] == '0' && a.size() > 1) {
        return false;
    }
    if (b.size() > 0) {
        sstream << '.' << b;
    }
    result = std::stold(sstream.str());
    return true;
}

//TODO: dokładność 0.001
bool comparator(const contribution &first, const contribution &second) {
    long double firstAmount = std::get<amount>(first) * currencies[std::get<currency>(first)];
    long double secondAmount = std::get<amount>(second) * currencies[std::get<currency>(second)];

    if (firstAmount != secondAmount) {
        return firstAmount < secondAmount;
    }

    return std::get<number>(first) < std::get<number>(second); // sortowanie po numerze z wejścia
}

bool checkPhase1(const std::string &line) {
    const static boost::regex pattern(R"(\s*(\u{3})\s+((\d+)(,(\d{1,3}))?)\s*)");
    bool matched = boost::regex_match(line, pattern);
    if (matched) {
        boost::smatch result;
        boost::regex_search(line, result, pattern);
        std::string currencyCode = result[1];
        long double currencyValue;
        if (!makeNumber(result[3], result[5], currencyValue)) {
            reportError(line);
            return false;
        }
        //TODO: usunąć następną linijkę na koniec
        //std::cerr << currencyCode << " = " << currencyValue << '\n';
        if (currencyValue <= 0.0 || currencies.find(currencyCode) != currencies.end()) {
            reportError(line);
        }
        else if (!error) {
            currencies[currencyCode] = currencyValue;
        }
    }

    return matched;
}

bool checkPhase2(const std::string &line) {
    const static boost::regex pattern(R"(\s*(.*?)\s+((\d+)(,(\d{1,3}))?)\s(\u{3})\s*)");
    bool matched = boost::regex_match(line, pattern);
    if (matched) {
        boost::smatch result;
        boost::regex_search(line, result, pattern);
        std::string name = result[1];
        long double amount;
        if (!makeNumber(result[3], result[5], amount)) {
            reportError(line);
            return false;
        }
        std::string currencyCode = result[6];
        //TODO: usunąć następną linijkę na koniec
        //std::cerr << "PHASE2 " << "|" << name << "|" << ' ' << amount << ' ' << currencyCode << '\n';
        if (amount <= 0.0 || currencies.find(currencyCode) == currencies.end()) {
            reportError(line);
            return false;
        }
        contribution con = std::make_tuple(name, amount, currencyCode, currentLineNumber);
        contributions.push_back(con);
    }
    return matched;
}

//TODO: usunąć tę funkcję na koniec
void printAll() {
    std::cout << "--- Print all --- \n";
    for (auto x : contributions) {
        std::cout << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << " " << std::get<3>(x) << "\n";
    }
    std::cout << "--- Print all end ---" << "\n";
}

bool query(long double begin, long double end) {
    if (begin > end) {
        return false;
    }

    //TODO: usunąć następną linijkę na koniec
    //printAll();

    contribution lowerBound = std::make_tuple("", begin, "", 0);
    contribution upperBound = std::make_tuple("", end, "", currentLineNumber + 1);

    conIterator lower = std::lower_bound(contributions.begin(), contributions.end(), lowerBound, comparator);
    conIterator upper = std::upper_bound(contributions.begin(), contributions.end(), upperBound, comparator);

    for (conIterator it = lower; it != upper; it++) {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\"" << std::get<0>(*it) << "\"";
        std::cout << ",\"" << std::get<1>(*it) << "\"";
        std::cout << "," << std::get<2>(*it) << "\n";
    }

    return true;
}

bool checkPhase3(const std::string &line) {
    const static boost::regex pattern(R"(\s*(\d+)(,(\d{1,3}))?\s+(\d+)(,(\d{1,3}))?\s*)");
    bool matched = boost::regex_match(line, pattern);
    if (matched) {
        if (!isSorted) {
            std::sort(contributions.begin(), contributions.end(), comparator);
            isSorted = true;
        }
        boost::smatch result;
        boost::regex_search(line, result, pattern);
        long double begin, end;
        if (!makeNumber(result[1], result[3], begin) || !makeNumber(result[4], result[6], end)) {
            reportError(line);
            return false;
        }
        if (!query(begin, end)) {
            reportError(line);
            return false;
        }
        //TODO: usunąć następną linijkę na koniec
        //std::cerr << "BEGIN------------------------------ " << begin << ' ' << end << '\n';
    }
    return matched;
}

void solve() {
    currencies[""] = 1.0; // pusty string to uniwersalna waluta
    std::array<std::function<bool(const std::string &)>, 3> phases
            {{checkPhase1, checkPhase2, checkPhase3}};
    std::string line;
    size_t currentPhase = 0;
    for (currentLineNumber = 1; std::getline(std::cin, line); currentLineNumber++) {
        bool success = false;
        error = false;
        for (size_t i = currentPhase; i < phases.size() && !success; i++) {
            if (phases[i](line)) {
                currentPhase = i;
                success = true;
            }
        }
        if (!success) {
            reportError(line);
        }
        //TODO: usunąć następną linijkę na koniec
        //std::cerr << line << "++++++++++" << currentPhase << '\n';
    }
}

//TODO: usunąć tę funkcję na koniec
void check(const std::string &line, std::function<bool(const std::string &)> foo) {
    if (foo(line)) {
        std::cout << line << " is valid\n";
    }
    else {
        std::cout << line << " is !!invalid\n";
    }
}

//TODO: usunąć tę funkcję na koniec
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
    check("Ala D   5,01 PLN", checkPhase2);
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

//TODO: usunąć tę funkcję na koniec
void roundTest(double x) {
    std::cout << "ROUND " << x << " " << nearbyint(x) << std::endl;
}

//TODO: usunąć tę funkcję na koniec
void roundTests() {
    roundTest(-5.5);
    roundTest(-6.5);
    roundTest(4.5);
    roundTest(5.5);
    roundTest(6.5);
    roundTest(5.1);
    roundTest(5.8);
}

int main() {
    //tests();
    solve();
}
