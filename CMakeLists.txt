cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_CXX_STANDARD 20)

project(graphics_test)
add_executable(${PROJECT_NAME} source/main.cpp)

set(INSTALL_PATH bin/graphics_test)

add_subdirectory(libutilities)
set(LIBGRAPHICS_INSTALL_PATH ${INSTALL_PATH})
add_subdirectory(libgraphics)

get_target_property(LIBGRAPHICS_INCLUDE_DIRECTORIES libgraphics INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBGRAPHICS_INCLUDE_DIRECTORIES})

target_link_libraries(${PROJECT_NAME} libgraphics)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_PATH})
