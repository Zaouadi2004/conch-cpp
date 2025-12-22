project(network)

# Create library (header-only placeholder for now)
add_library(${PROJECT_NAME} INTERFACE)

# Include paths
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        libuv::uv_a
        foundation
)
