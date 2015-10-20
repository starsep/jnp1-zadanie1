//Filip Czaplicki, Bartosz Łukasiewicz 
#include <boost/regex.hpp>
#include <cctype>
#include <iostream>
#include <sstream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <iomanip>
#include <exception>

//Wartość wpłaty przechowujemy jako parę (liczba przed przecinkiem, liczba po przecinku).
typedef std::pair<long long, long long> numeralType;

//Wpłatę przechowujemy jako czwórkę (nazwa, kwota, waluta, numer wpłaty).
typedef std::tuple<std::string, numeralType, std::string, int> contribution;

typedef std::vector<contribution>::iterator conIterator;

enum {
    name = 0,
    amount = 1,
    currency = 2,
    number = 3
};

const long long ten = 10;
const long long precision = 1000;
const long long precision2 = precision * precision;

bool isSorted = false;
bool error = false;
int currentLineNumber;
std::map<std::string, numeralType> currencies; //przelicznik dla kodu waluty
std::vector <contribution> contributions;

numeralType operator*(const numeralType &a, const numeralType &b) {
	numeralType result;
	
	long long valueA = precision * a.first + a.second;
	long long valueB = precision * b.first + b.second;
	
	long long product = valueA * valueB;
	result.first = product / precision2;
	
	long long remainder = product % precision2;
	result.second = remainder / precision;
	
	long long significantDigit = remainder / (precision / ten) % ten;
	
	if ((significantDigit == 5 && result.second % 2 == 1) || (significantDigit > 5)) {
		result.second++;
	}

	return result;
}

std::ostream& operator<<(std::ostream& os, const numeralType &num) {
    os << num.first << ',' << std::setfill('0') << std::setw(3) << num.second;
    return os;
}

void reportError(const std::string &line) {
    if (!error) {
        std::cerr << "Error in line " << currentLineNumber << ':' << line << '\n';
        error = true;
    }
}

//ZAŁOŻENIE: zera wiodące to błąd, część po przecinku ma nie więcej niż 3 cyfry
//tworzy liczbę z części przed przecinkiem i po
bool makeNumber(const std::string &a, const std::string &b, numeralType &result) {
    std::string bWithZeroes = b;
    while(bWithZeroes.size() < 3) {
		bWithZeroes += "0";
	}
    if (a[0] == '0' && a.size() > 1) {
        return false;
    }
    result.first = std::stoll(a);
    result.second = std::stoll(bWithZeroes);
    
    return true;
}

bool comparator(const contribution &first, const contribution &second) {
    numeralType firstAmount = std::get<amount>(first) * currencies[std::get<currency>(first)];
    numeralType secondAmount = std::get<amount>(second) * currencies[std::get<currency>(second)];

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
        numeralType currencyValue;
        bool correctCurrencyValue = makeNumber(result[3], result[5], currencyValue);
        if (!correctCurrencyValue || currencyValue == numeralType(0,0)) {
            reportError(line);
            return false;
        }

        //jeśli znamy już tę walutę i ma ona inny kurs, to wypisujemy error
        //zakładamy, że dwukrotny (ten sam) kurs tej samej waluty jest poprawny
        if (currencies.find(currencyCode) != currencies.end() && currencies[currencyCode] != currencyValue) {
            reportError(line);
        }
        else if (!error) {
            currencies[currencyCode] = currencyValue;
        }
    }

    return matched;
}

bool checkPhase2(const std::string &line) {
    const static boost::regex pattern(R"(\s*(.*?)\s+((\d+)(,(\d{1,3}))?)\s+(\u{3})\s*)");
    bool matched = boost::regex_match(line, pattern);
    if (matched) {
        boost::smatch result;
        boost::regex_search(line, result, pattern);
        std::string name = result[1];
        numeralType amount;
        bool correctAmount = makeNumber(result[3], result[5], amount);
        std::string currencyCode = result[6];
        if (!correctAmount || currencies.find(currencyCode) == currencies.end()) {
            reportError(line);
            return false;
        }
        contribution con = std::make_tuple(name, amount, currencyCode, currentLineNumber);
        contributions.push_back(con);
    }
    return matched;
}

bool query(numeralType begin, numeralType end) {
    if (begin > end) {
        return false;
    }

    contribution lowerBound = std::make_tuple("", begin, "", 0);
    contribution upperBound = std::make_tuple("", end, "", currentLineNumber + 1);

    conIterator lower = std::lower_bound(contributions.begin(), contributions.end(), lowerBound, comparator);
    conIterator upper = std::upper_bound(contributions.begin(), contributions.end(), upperBound, comparator);

    for (conIterator it = lower; it != upper; it++) {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "\"" << std::get<name>(*it) << "\"";
        std::cout << ",\"" << std::get<amount>(*it) << "\"";
        std::cout << "," << std::get<currency>(*it) << "\n";
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
        numeralType begin, end;
        bool correctBegin = makeNumber(result[1], result[3], begin);
        bool correctEnd = makeNumber(result[4], result[6], end);
        if (!correctBegin || !correctEnd || !query(begin, end)) {
            reportError(line);
            return false;
        }
    }
    return matched;
}

void solve() {
    currencies[""] = numeralType(1, 0); // pusty string to uniwersalna waluta
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
    }
}

int main() {
    solve();
}
