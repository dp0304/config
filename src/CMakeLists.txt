#### building library
FILE(GLOB_RECURSE HEADERS *.h *.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
FILE(GLOB_RECURSE SOURCES *.cpp)
message(STATUS "HEADERS : ${HEADERS}")
message(STATUS "SOURCES : ${SOURCES}")
add_library(ezconfig STATIC ${HEADERS} ${SOURCES})
target_compile_features(ezconfig PUBLIC cxx_std_17)

#### adding dependency within module
target_include_directories(ezconfig PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")