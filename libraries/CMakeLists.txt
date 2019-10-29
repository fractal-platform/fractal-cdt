cmake_minimum_required(VERSION 3.5)
project(ftl_libraries)

set(BASE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/../run")

set(CMAKE_RANLIB "${BASE_BINARY_DIR}/bin/llvm-ranlib")
set(CMAKE_AR "${BASE_BINARY_DIR}/bin/llvm-ar")
set(CMAKE_C_COMPILER "${BASE_BINARY_DIR}/bin/fractal-cc")
set(CMAKE_CXX_COMPILER "${BASE_BINARY_DIR}/bin/fractal-cpp")
set(CMAKE_ASM_COMPILER "${BASE_BINARY_DIR}/bin/fractal-cc")

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O3 -Wall ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O3 -Wall ")
set(CMAKE_ASM_FLAGS " -fnative -fasm ")

set(WASM_LINKER "${BASE_BINARY_DIR}/bin/wasm-ld")
set(CMAKE_C_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_subdirectory(libc)
add_subdirectory(libc++)
add_subdirectory(ftllib)
add_subdirectory(boost)
