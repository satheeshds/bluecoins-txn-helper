cmake_minimum_required(VERSION 3.11)

project(blueConvert VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)

FetchContent_Declare(
  SQLiteCpp
  GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
  GIT_TAG origin/master
)

# FetchContent_Declare(
#     OpenXLSX
#     GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
#     GIT_TAG master
# )

# FetchContent_Declare(
#     BasicExcel
#     GIT_REPOSITORY https://github.com/mimengdelei/BasicExcel.git
#     GIT_BRANCH master
# )

FetchContent_MakeAvailable(CLI11 SQLiteCpp)

find_package(Curses REQUIRED)

include_directories(./include)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/globals.cpp src/db.cpp)

target_link_libraries(${PROJECT_NAME} CLI11::CLI11 SQLiteCpp ${CURSES_LIBRARIES})