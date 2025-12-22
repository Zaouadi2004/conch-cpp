add_library(ui_kit INTERFACE)

# Qt6 is required
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

target_link_libraries(ui_kit INTERFACE
    Qt6::Core Qt6::Gui Qt6::Widgets
    foundation
)
