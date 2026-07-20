cmake_minimum_required(VERSION 3.20)

project(ZoneEyeAgent)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)
find_package(Tesseract CONFIG REQUIRED)

add_executable(
    ZoneEyeAgent
    src/main.cpp
    src/Application.cpp
    src/Config.cpp
    src/Logger.cpp
    src/HeartbeatManager.cpp
    src/CaptureManager.cpp
    src/UploadManager.cpp
    src/HttpClient.cpp
    src/OCRManager.cpp
    src/GameState.cpp
    src/GameStateParser.cpp
    src/GameStateManager.cpp
    src/MatchSession.cpp
    src/SessionManager.cpp
    src/MatchTracker.cpp
    src/
)
target_link_libraries(
    ZoneEyeAgent
    PRIVATE
    ${OpenCV_LIBS}
    CURL::libcurl
    Tesseract::libtesseract
)
