#include <boost/regex.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

bool isISO4217valid(const std::string &code) {
    boost::regex pattern("[A-Z]{3}");
    return true;
}

int main() {

}
