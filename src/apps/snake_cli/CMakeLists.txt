project(snake_cli)

add_executable(${PROJECT_NAME}
    main.cpp
    snake_game.cpp
    snake_game.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        foundation
        spdlog::spdlog
)
