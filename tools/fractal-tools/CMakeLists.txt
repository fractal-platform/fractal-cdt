cmake_minimum_required(VERSION 3.5)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
   message(STATUS "Using ccache")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLVM_SRCDIR}/tools/clang/include)
include_directories(${LLVM_BINDIR}/tools/clang/include)
include_directories(${LLVM_SRCDIR}/include)
include_directories(${LLVM_BINDIR}/include)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(FTL_STACK_SIZE 8192)

macro (add_tool name)
   set(LLVM_LINK_COMPONENTS support)

   include_directories(include)
   
   add_executable(${name} ${CMAKE_BINARY_DIR}/${name}.cpp)
   set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)
   target_compile_options(${name} PRIVATE -fexceptions -fno-rtti)
   target_include_directories(${name} PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/../jsoncons/include ${CMAKE_SOURCE_DIR}/llvm/tools/clang/include ${LLVM_INCLUDE_DIR})
   target_link_libraries(${name}
     clangTooling
     clangBasic
     clangASTMatchers
     clangRewrite
     clangToolingCore
     clangFrontend
     clangDriver
     clangSerialization
     clangParse
     clangSema
     clangAnalysis
     clangAST
     clangLex
     clangBasic
     clangEdit

     LLVMipo
     LLVMScalarOpts
     LLVMInstCombine
     LLVMTransformUtils
     LLVMAnalysis
     LLVMTarget
     LLVMOption
     LLVMMCParser
     LLVMMC
     LLVMObject
     LLVMBitReader
     LLVMCore
     LLVMSupport
     LLVMDemangle
   )
endmacro()

add_subdirectory(cc)
add_subdirectory(ld)
add_subdirectory(external)
