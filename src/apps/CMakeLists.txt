# CMake configuration for apps directory
# Desktop: Qt-based cross-platform app
# Mobile: Native iOS and Android apps

add_subdirectory(desktop_pro)
add_subdirectory(ios_native)

# Android app requires Android NDK
if(ANDROID)
    add_subdirectory(android_native)
endif()
