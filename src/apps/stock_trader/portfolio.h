#pragma once
#include "market_data.h"
#include <unordered_map>
#include <vector>

struct Position {
    std::string symbol;
    int shares;
    double avg_cost;
    
    double getCurrentValue(double current_price) const {
        return shares * current_price;
    }
    
    double getProfitLoss(double current_price) const {
        return (current_price - avg_cost) * shares;
    }
};

class Portfolio {
public:
    Portfolio(double initial_cash = 100000.0);
    
    bool buy(const std::string& symbol, int shares, double price);
    bool sell(const std::string& symbol, int shares, double price);
    
    void displayPortfolio(MarketData& market);
    void displayPositions(MarketData& market);
    
    double getCash() const { return cash_; }
    double getTotalValue(MarketData& market) const;
    
private:
    double cash_;
    std::unordered_map<std::string, Position> positions_;
    std::vector<std::string> transaction_history_;
};
