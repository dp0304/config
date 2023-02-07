#### building library
FILE(GLOB_RECURSE SOURCES *.cpp)
message(STATUS "SOURCES : ${SOURCES}")
add_executable(ezconfig_runnable ${SOURCES})
target_compile_features(ezconfig PUBLIC cxx_std_17)

#### adding dependency within module
target_include_directories(ezconfig_runnable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(ezconfig_runnable PRIVATE ezconfig)