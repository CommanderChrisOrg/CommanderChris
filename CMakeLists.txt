cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# --- Fetch spdlog -------------------------------------------------------------
FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(spdlog)

# ------------------------------------------------------------------------------

project(chris
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(chris src/main.cpp src/gpt.cpp src/scrap.cpp)
target_include_directories(chris PRIVATE src)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(chris
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE spdlog::spdlog
  ${CURL_LIBRARIES}
)
