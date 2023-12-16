project(toytls_demo)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES ./*.cpp)
file(GLOB HEAD_FILES ./*.h)

find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})

target_link_libraries(${PROJECT_NAME} toytls Boost::system Boost::thread)
