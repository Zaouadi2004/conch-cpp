#include "market_data.h"
#include <spdlog/spdlog.h>

MarketData::MarketData() 
    : rng_(std::random_device{}()), 
      price_change_(-0.02, 0.02) {
    
    // Initialize with popular US stocks
    stocks_["AAPL"] = {"AAPL", 185.50, 0.0};
    stocks_["MSFT"] = {"MSFT", 380.20, 0.0};
    stocks_["GOOGL"] = {"GOOGL", 140.75, 0.0};
    stocks_["AMZN"] = {"AMZN", 155.30, 0.0};
    stocks_["TSLA"] = {"TSLA", 245.60, 0.0};
    stocks_["NVDA"] = {"NVDA", 495.80, 0.0};
    stocks_["META"] = {"META", 355.25, 0.0};
    stocks_["NFLX"] = {"NFLX", 485.90, 0.0};
    
    spdlog::info("Market data initialized with {} stocks", stocks_.size());
}

Stock MarketData::getQuote(const std::string& symbol) {
    auto it = stocks_.find(symbol);
    if (it != stocks_.end()) {
        return it->second;
    }
    return {"", 0.0, 0.0};
}

void MarketData::updatePrices() {
    for (auto& [symbol, stock] : stocks_) {
        double change = price_change_(rng_);
        stock.price *= (1.0 + change);
        stock.change_percent = change * 100.0;
    }
}
