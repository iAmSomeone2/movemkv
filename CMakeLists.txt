cmake_minimum_required(VERSION 3.10)
project(movemkv VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3")

find_package(Threads REQUIRED)

add_executable(movemkv movemkv.cxx movemkv.hxx)
target_link_libraries(movemkv PUBLIC Threads::Threads)
target_compile_options(movemkv PRIVATE -Wall)
target_compile_features(movemkv PUBLIC cxx_auto_type cxx_range_for)

# Deal with installation
include(GNUInstallDirs)
install(TARGETS movemkv RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/movemkv)