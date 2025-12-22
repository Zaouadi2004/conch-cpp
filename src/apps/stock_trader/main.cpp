#include "market_data.h"
#include "portfolio.h"
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

void displayMenu() {
    fmt::print("\n=== STOCK TRADING SIMULATOR ===\n");
    fmt::print("1. View Market Prices\n");
    fmt::print("2. View Portfolio\n");
    fmt::print("3. View Positions\n");
    fmt::print("4. Buy Stock\n");
    fmt::print("5. Sell Stock\n");
    fmt::print("6. Update Prices (Simulate)\n");
    fmt::print("0. Exit\n");
    fmt::print("==============================\n");
    fmt::print("Choice: ");
}

void displayMarket(MarketData& market) {
    std::vector<std::string> symbols = {"AAPL", "MSFT", "GOOGL", "AMZN", 
                                        "TSLA", "NVDA", "META", "NFLX"};
    
    fmt::print("\n========== MARKET QUOTES ==========\n");
    fmt::print("{:<8} {:>12} {:>12}\n", "Symbol", "Price", "Change %");
    fmt::print("{}\n", std::string(34, '-'));
    
    for (const auto& sym : symbols) {
        Stock stock = market.getQuote(sym);
        std::string change_str = fmt::format("{:+.2f}%", stock.change_percent);
        fmt::print("{:<8} ${:>11.2f} {:>12}\n", 
                  stock.symbol, stock.price, change_str);
    }
    fmt::print("===================================\n\n");
}

int main() {
    spdlog::info("Starting Stock Trading Simulator...");
    
    MarketData market;
    Portfolio portfolio(100000.0);  // $100,000 starting cash
    
    fmt::print("\n🎯 Welcome to Stock Trading Simulator!\n");
    fmt::print("Starting balance: $100,000.00\n");
    fmt::print("Available stocks: AAPL, MSFT, GOOGL, AMZN, TSLA, NVDA, META, NFLX\n");
    
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            fmt::print("Invalid input!\n");
            continue;
        }
        
        switch (choice) {
            case 0:
                running = false;
                fmt::print("\nThank you for trading! Final portfolio value: ${:.2f}\n", 
                          portfolio.getTotalValue(market));
                break;
                
            case 1:
                displayMarket(market);
                break;
                
            case 2:
                portfolio.displayPortfolio(market);
                break;
                
            case 3:
                portfolio.displayPositions(market);
                break;
                
            case 4: {
                std::string symbol;
                int shares;
                fmt::print("\nEnter symbol (e.g., AAPL): ");
                std::cin >> symbol;
                fmt::print("Enter number of shares: ");
                std::cin >> shares;
                
                Stock stock = market.getQuote(symbol);
                if (stock.price == 0.0) {
                    fmt::print("Unknown symbol: {}\n", symbol);
                } else {
                    double cost = shares * stock.price;
                    fmt::print("Buy {} shares of {} @ ${:.2f} = ${:.2f}\n", 
                              shares, symbol, stock.price, cost);
                    fmt::print("Confirm? (y/n): ");
                    char confirm;
                    std::cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        if (portfolio.buy(symbol, shares, stock.price)) {
                            fmt::print("✅ Order executed!\n");
                        } else {
                            fmt::print("❌ Order failed!\n");
                        }
                    }
                }
                break;
            }
                
            case 5: {
                std::string symbol;
                int shares;
                fmt::print("\nEnter symbol: ");
                std::cin >> symbol;
                fmt::print("Enter number of shares: ");
                std::cin >> shares;
                
                Stock stock = market.getQuote(symbol);
                if (stock.price == 0.0) {
                    fmt::print("Unknown symbol: {}\n", symbol);
                } else {
                    double proceeds = shares * stock.price;
                    fmt::print("Sell {} shares of {} @ ${:.2f} = ${:.2f}\n", 
                              shares, symbol, stock.price, proceeds);
                    fmt::print("Confirm? (y/n): ");
                    char confirm;
                    std::cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        if (portfolio.sell(symbol, shares, stock.price)) {
                            fmt::print("✅ Order executed!\n");
                        } else {
                            fmt::print("❌ Order failed!\n");
                        }
                    }
                }
                break;
            }
                
            case 6:
                market.updatePrices();
                fmt::print("📈 Market prices updated!\n");
                displayMarket(market);
                break;
                
            default:
                fmt::print("Invalid choice!\n");
                break;
        }
    }
    
    spdlog::info("Stock Trading Simulator terminated");
    return 0;
}
