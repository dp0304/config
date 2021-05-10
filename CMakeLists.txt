cmake_minimum_required(VERSION 3.17)

project(ezconfig_proj
        VERSION 0.0.1
        DESCRIPTION "ezconfig project"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

## VS cmake option
if (MSVC)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # IDEs should put the headers in a nice place
    source_group(
            TREE "${PROJECT_SOURCE_DIR}/include"
            PREFIX "Header Files"
            FILES ${HEADER_LIST})
endif()

add_subdirectory(src)
#add_subdirectory(runnable)
