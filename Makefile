all: opp.cc
	g++ -g -std=c++11 -O2 -Wall -pedantic -Wunused -Wshadow -o "opp" "opp.cc" -lboost_regex
