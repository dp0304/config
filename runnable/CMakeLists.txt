#### building library
FILE(GLOB_RECURSE HEADERS *.h *.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
FILE(GLOB_RECURSE SOURCES *.cpp)
message(STATUS "HEADERS : ${HEADERS}")
message(STATUS "SOURCES : ${SOURCES}")
add_executable(ezconfig_runnable ${HEADERS} ${SOURCES})

#### adding dependency within module
target_include_directories(ezconfig_runnable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(ezconfig_runnable PRIVATE ezconfig)