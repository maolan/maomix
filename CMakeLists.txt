cmake_minimum_required(VERSION 3.10)
project(maomix VERSION 0.0.1 DESCRIPTION "Maolan Mixer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include(GNUInstallDirs)

file(GLOB SRCS src/*.cpp imgui-knobs/*.cpp imnodes/*.cpp)
file(GLOB HEADERS ${CMAKE_PROJECT_NAME}/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME})
add_subdirectory(imguiwrap)
enable_testing()

find_package(PkgConfig REQUIRED)

pkg_check_modules(LO REQUIRED liblo)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${LO_INCLUDE_DIRS})
set(LIBRARY_DIRS ${LIBRARY_DIRS} ${LO_LIBRARY_DIRS})
set(LIBRARIES ${LIBRARIES} ${LO_LIBRARIES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-knobs
  ${CMAKE_CURRENT_SOURCE_DIR}/imnodes
  ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(${CMAKE_PROJECT_NAME}-bin ${SRCS} ${IMGUI_SRCS} ${HEADERS})
set_target_properties(${CMAKE_PROJECT_NAME}-bin PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
target_link_libraries(${CMAKE_PROJECT_NAME}-bin ${LIBRARIES} ${CMAKE_DL_LIBS} imguiwrap)
target_link_directories(${CMAKE_PROJECT_NAME}-bin PUBLIC ${LIBRARY_DIRS})
install(TARGETS ${CMAKE_PROJECT_NAME}-bin RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
