cmake_minimum_required (VERSION 3.11)
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(BUILD_SHARED_LIBS OFF)

# --- Fetch FTXUI --------------------------------------------------------------

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

# --- Fetch json ---------------------------------------------------------------

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

# ------------------------------------------------------------------------------

project(chris
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(chris src/main.cpp src/gpt.cpp src/context.cpp)
target_include_directories(chris PRIVATE src)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(chris
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
  ${CURL_LIBRARIES}
)
