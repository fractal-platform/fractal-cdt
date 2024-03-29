cmake_minimum_required(VERSION 3.5)
project(fractal_cdt_tools)

include(ExternalProject)
find_package(Git REQUIRED)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 3)
set(VERSION_PATCH 0)
#set(VERSION_SUFFIX rc1)

set(BASE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/../run")

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

ExternalProject_Add(
        FractalClang
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DFRACTAL_TOOL_DIR=${CMAKE_SOURCE_DIR}/fractal-tools

        SOURCE_DIR "${CMAKE_SOURCE_DIR}/llvm"
        BINARY_DIR "${CMAKE_BINARY_DIR}/llvm"
        UPDATE_COMMAND ""
        PATCH_COMMAND  ""
        TEST_COMMAND   ""
        INSTALL_COMMAND ""
        BUILD_ALWAYS 1
)

set(LLVM_BINDIR ${CMAKE_BINARY_DIR}/llvm)
ExternalProject_Add(
        FractalTools
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DVERSION_FULL=${VERSION_FULL} -DLLVM_SRCDIR=${CMAKE_SOURCE_DIR}/llvm -DLLVM_BINDIR=${LLVM_BINDIR} -DLLVM_DIR=${LLVM_BINDIR}/lib/cmake/llvm

        SOURCE_DIR "${CMAKE_SOURCE_DIR}/fractal-tools"
        BINARY_DIR "${CMAKE_BINARY_DIR}/fractal-tools"
        UPDATE_COMMAND ""
        PATCH_COMMAND  ""
        TEST_COMMAND   ""
        INSTALL_COMMAND ""
        BUILD_ALWAYS 1
        DEPENDS FractalClang
)


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/clang-7 PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/lld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/wasm-ld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ar PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ranlib PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/lib/LLVMFtlFixup.so PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cc PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cpp PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/ld/fractal-ld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-pp PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wasm2wast PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wast2wasm PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/clang.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/clang-7)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/lld.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/wasm-ld.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ar.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ranlib.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cc.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cpp.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/ld/fractal-ld.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-pp.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wasm2wast.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wast2wasm.exe PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/clang-7 PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/lld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/wasm-ld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ar PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/bin/llvm-ranlib PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/llvm/lib/LLVMFtlFixup.dylib PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cc PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/cc/fractal-cpp PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/ld/fractal-ld PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-pp PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wasm2wast PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/fractal-tools/external/wabt/fractal-wast2wasm PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ DESTINATION ${BASE_BINARY_DIR}/bin/)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

