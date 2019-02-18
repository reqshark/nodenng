cmake_minimum_required (VERSION 3.1)
project (nodenng)
file(GLOB SOURCE_FILES "src/nodenng.cc")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

#add_dependencies(nodenng nng)
#add_library(nng_dep)
#add_subdirectory(nng)
#target_sources(nng_dep PRIVATE nng)

add_library(nng OBJECT IMPORTED)
#link_directories(nng)
target_link_libraries(nodenng nng)
