cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(SuperK)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES "*.mac")

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(sk2 sk2.cc ${sources} ${headers})
target_include_directories(sk2 PRIVATE include)
target_link_libraries(sk2 ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS sk2)
