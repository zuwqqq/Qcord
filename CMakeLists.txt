cmake_minimum_required(VERSION 3.16)
project(Qcord VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Options
option(ENABLE_TWITCH "Enable Twitch support" ON)
option(ENABLE_YOUTUBE "Enable YouTube support" ON)
option(ENABLE_TELEGRAM "Enable Telegram support" ON)

# Find packages
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(FFmpeg REQUIRED COMPONENTS LIBAVFORMAT LIBAVCODEC LIBAVUTIL LIBSWSCALE)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/core/encoder.cpp
    src/core/source.cpp
    src/core/output.cpp
    src/filters/filter.cpp
    src/ui/mainwindow.cpp
    src/ui/settingsdialog.cpp
)

set(HEADERS
    src/core/encoder.h
    src/core/source.h
    src/core/output.h
    src/core/broadcast.h
    src/filters/filter.h
    src/ui/mainwindow.h
    src/ui/settingsdialog.h
)

# Streaming modules
if(ENABLE_TWITCH)
    list(APPEND SOURCES src/outputs/twitch/twitch_output.cpp)
    list(APPEND HEADERS src/outputs/twitch/twitch_output.h)
    add_compile_definitions(ENABLE_TWITCH)
endif()

if(ENABLE_YOUTUBE)
    list(APPEND SOURCES src/outputs/youtube/youtube_output.cpp)
    list(APPEND HEADERS src/outputs/youtube/youtube_output.h)
    add_compile_definitions(ENABLE_YOUTUBE)
endif()

if(ENABLE_TELEGRAM)
    list(APPEND SOURCES src/outputs/telegram/telegram_output.cpp)
    list(APPEND HEADERS src/outputs/telegram/telegram_output.h)
    add_compile_definitions(ENABLE_TELEGRAM)
endif()

# Create executable
add_executable(qcord ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(qcord
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${FFMPEG_LIBRARIES}
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Include directories
target_include_directories(qcord PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

# Installation
install(TARGETS qcord DESTINATION bin)
