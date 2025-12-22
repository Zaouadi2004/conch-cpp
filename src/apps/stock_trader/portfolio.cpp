#include "portfolio.h"
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <iostream>
#include <iomanip>

Portfolio::Portfolio(double initial_cash) : cash_(initial_cash) {
    spdlog::info("Portfolio initialized with ${:.2f}", cash_);
}

bool Portfolio::buy(const std::string& symbol, int shares, double price) {
    double cost = shares * price;
    
    if (cost > cash_) {
        spdlog::warn("Insufficient funds to buy {} shares of {}", shares, symbol);
        return false;
    }
    
    cash_ -= cost;
    
    auto& pos = positions_[symbol];
    if (pos.shares > 0) {
        // Update average cost
        double total_cost = (pos.avg_cost * pos.shares) + cost;
        pos.shares += shares;
        pos.avg_cost = total_cost / pos.shares;
    } else {
        pos.symbol = symbol;
        pos.shares = shares;
        pos.avg_cost = price;
    }
    
    std::string tx = fmt::format("BUY {} x{} @ ${:.2f}", symbol, shares, price);
    transaction_history_.push_back(tx);
    spdlog::info("{}", tx);
    
    return true;
}

bool Portfolio::sell(const std::string& symbol, int shares, double price) {
    auto it = positions_.find(symbol);
    if (it == positions_.end() || it->second.shares < shares) {
        spdlog::warn("Insufficient shares to sell {} of {}", shares, symbol);
        return false;
    }
    
    cash_ += shares * price;
    it->second.shares -= shares;
    
    if (it->second.shares == 0) {
        positions_.erase(it);
    }
    
    std::string tx = fmt::format("SELL {} x{} @ ${:.2f}", symbol, shares, price);
    transaction_history_.push_back(tx);
    spdlog::info("{}", tx);
    
    return true;
}

void Portfolio::displayPortfolio(MarketData& market) {
    std::cout << "\n========== PORTFOLIO ==========\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Cash: $" << cash_ << "\n";
    
    double holdings_value = 0.0;
    for (const auto& [symbol, pos] : positions_) {
        Stock stock = market.getQuote(symbol);
        holdings_value += pos.getCurrentValue(stock.price);
    }
    
    double total = cash_ + holdings_value;
    std::cout << "Holdings: $" << holdings_value << "\n";
    std::cout << "Total Value: $" << total << "\n";
    std::cout << "P&L: $" << (total - 100000.0) << "\n";
    std::cout << "==============================\n\n";
}

void Portfolio::displayPositions(MarketData& market) {
    if (positions_.empty()) {
        std::cout << "No positions.\n";
        return;
    }
    
    std::cout << "\n========== POSITIONS ==========\n";
    std::cout << std::left << std::setw(8) << "Symbol" 
              << std::right << std::setw(10) << "Shares"
              << std::setw(12) << "Avg Cost"
              << std::setw(12) << "Current"
              << std::setw(12) << "Value"
              << std::setw(12) << "P&L" << "\n";
    std::cout << std::string(66, '-') << "\n";
    
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& [symbol, pos] : positions_) {
        Stock stock = market.getQuote(symbol);
        double value = pos.getCurrentValue(stock.price);
        double pl = pos.getProfitLoss(stock.price);
        
        std::cout << std::left << std::setw(8) << symbol
                  << std::right << std::setw(10) << pos.shares
                  << std::setw(12) << pos.avg_cost
                  << std::setw(12) << stock.price
                  << std::setw(12) << value
                  << std::setw(12) << pl << "\n";
    }
    std::cout << "===============================\n\n";
}

double Portfolio::getTotalValue(MarketData& market) const {
    double total = cash_;
    for (const auto& [symbol, pos] : positions_) {
        Stock stock = market.getQuote(symbol);
        total += pos.getCurrentValue(stock.price);
    }
    return total;
}
