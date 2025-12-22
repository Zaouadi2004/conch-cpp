#pragma once
#include <string>
#include <unordered_map>
#include <random>

struct Stock {
    std::string symbol;
    double price;
    double change_percent;
};

class MarketData {
public:
    MarketData();
    
    Stock getQuote(const std::string& symbol);
    void updatePrices(); // Simulate price changes
    
private:
    std::unordered_map<std::string, Stock> stocks_;
    std::mt19937 rng_;
    std::uniform_real_distribution<> price_change_;
};
