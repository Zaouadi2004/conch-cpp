project(stock_trader)

add_executable(${PROJECT_NAME}
    main.cpp
    market_data.cpp
    market_data.h
    portfolio.cpp
    portfolio.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        foundation
        spdlog::spdlog
        fmt::fmt
)
