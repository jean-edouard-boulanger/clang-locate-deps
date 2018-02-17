cmake_minimum_required(VERSION 3.4.3)
project(clang-locate-deps)

find_package(Clang REQUIRED CONFIG)
find_package(RapidJSON REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})
include(HandleLLVMOptions)

add_compile_options(-fno-rtti)
add_compile_options(-fno-exceptions)

file(GLOB SRC "src/*.cpp" "src/*.h")
add_executable(clang-locate-deps ${SRC})

include_directories(src ${LLVM_INCLUDE_DIRS} ${RapidJSON_INCLUDE_DIRS})
target_link_libraries(clang-locate-deps PRIVATE clangTooling)