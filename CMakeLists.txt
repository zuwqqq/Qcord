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
find_package(FFmpeg REQUIRED COMPONENTS LIBAVFORMAT LIBAVCODEC LIBAVUTIL LIBSWSCALE LIBSWRESAMPLE)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

# Source files - Core
set(CORE_SOURCES
    src/core/broadcast.cpp
    src/core/encoder.cpp
    src/core/source.cpp
    src/core/output.cpp
    src/filters/filter.cpp
    src/sources/screen_capture.cpp
    src/sources/webcam_capture.cpp
    src/sources/audio_input.cpp
)

# Platform-specific sources
if(WIN32)
    list(APPEND CORE_SOURCES src/platform/windows/dxgi_capture.cpp)
elseif(APPLE)
    list(APPEND CORE_SOURCES src/platform/macos/cg_capture.mm)
else()
    list(APPEND CORE_SOURCES src/platform/linux/x11_capture.cpp)
endif()

# UI Sources
set(UI_SOURCES
    src/ui/mainwindow.cpp
    src/ui/settingsdialog.cpp
)

# Streaming modules
set(OUTPUT_SOURCES
    src/outputs/twitch/twitch_output.cpp
    src/outputs/youtube/youtube_output.cpp
    src/outputs/telegram/telegram_output.cpp
    src/outputs/local/file_output.cpp
)

set(SOURCES ${CORE_SOURCES} ${UI_SOURCES} ${OUTPUT_SOURCES})

set(HEADERS
    src/core/broadcast.h
    src/core/encoder.h
    src/core/source.h
    src/core/output.h
    src/filters/filter.h
    src/ui/mainwindow.h
    src/ui/settingsdialog.h
    src/sources/screen_capture.h
    src/sources/webcam_capture.h
    src/sources/audio_input.h
    src/outputs/twitch/twitch_output.h
    src/outputs/youtube/youtube_output.h
    src/outputs/telegram/telegram_output.h
    src/outputs/local/file_output.h
)

# Add compile definitions for enabled features
if(ENABLE_TWITCH)
    add_compile_definitions(ENABLE_TWITCH)
endif()
if(ENABLE_YOUTUBE)
    add_compile_definitions(ENABLE_YOUTUBE)
endif()
if(ENABLE_TELEGRAM)
    add_compile_definitions(ENABLE_TELEGRAM)
endif()

# Main executable
add_executable(qcord src/main.cpp ${SOURCES} ${HEADERS})

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

# Platform-specific libraries
if(WIN32)
    target_link_libraries(qcord
        d3d11
        dxgi
        dxva2
        strmiids
        winmm
    )
elseif(APPLE)
    target_link_libraries(qcord
        "-framework CoreGraphics"
        "-framework AVFoundation"
        "-framework AudioToolbox"
    )
else()
    find_package(X11 REQUIRED)
    find_package(PulseAudio REQUIRED)
    target_link_libraries(qcord
        ${X11_LIBRARIES}
        ${PULSEAUDIO_LIBRARY}
    )
endif()

# Include directories
target_include_directories(qcord PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

# Installation
install(TARGETS qcord DESTINATION bin)