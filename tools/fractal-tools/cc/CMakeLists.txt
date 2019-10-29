configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fractal-cc.cpp.in ${CMAKE_BINARY_DIR}/fractal-cc.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fractal-cpp.cpp.in ${CMAKE_BINARY_DIR}/fractal-cpp.cpp)

add_tool(fractal-cc)
add_tool(fractal-cpp)
