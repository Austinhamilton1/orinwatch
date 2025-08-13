cmake_minimum_required(VERSION 3.14)
project(OrinWatch)

set(CMAKE_CXX_STANDARD 20)

add_executable(orinstats src/stats.cpp src/watcher.cpp)
target_include_directories(orinstats PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(orinstats PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(orinwatch src/orinwatch.cpp src/watcher.cpp src/buffer.cpp)
target_include_directories(orinwatch PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(orinwatch PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(TARGETS orinstats DESTINATION bin)
install(TARGETS orinwatch DESTINATION bin)

install(FILES orinwatch.service DESTINATION /etc/systemd/system)
