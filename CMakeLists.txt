cmake_minimum_required(VERSION 3.11)

project(blueConvert VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
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

FetchContent_MakeAvailable(CLI11)

include_directories(./include)
include_directories(/usr/local/include)
# link_directories(lib/BasicExcel)
find_library(LIBXLSREADER_LIBRARY NAMES xlsreader PATHS /usr/local/lib)
if(NOT LIBXLSREADER_LIBRARY)
    message(FATAL_ERROR "libxlsreader not found")
endif()


# find_package(libxls REQUIRED)
add_executable(${PROJECT_NAME} src/main.cpp src/BasicExcel.cpp src/globals.cpp)

target_link_libraries(${PROJECT_NAME} CLI11::CLI11 ${LIBXLSREADER_LIBRARY} )