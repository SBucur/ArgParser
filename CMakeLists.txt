cmake_minimum_required( VERSION 3.20 )
project( ARRRRRGS VERSION 0.1 )
# force CXX17 because we need std::filesystem
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED true )
set( CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install )

# include all the source files
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
)

# lump all the source files that aren't executable into a single variable
file ( GLOB my_SRCS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.hpp
)

# make it into a library
add_library( ArgParser
    ${my_SRCS}
)
# it depends on std::filesystem, but the compiler doesn't link it by default
target_link_libraries( ArgParser
    stdc++fs
)

# main is our executable
add_executable( ${PROJECT_NAME} main.cpp )
# link it all together!
target_link_libraries( ${PROJECT_NAME} ArgParser)

# put the output into a seperate directory
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
