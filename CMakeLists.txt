cmake_minimum_required(VERSION 3.14)
project(OrinWatch)

set(CMAKE_CXX_STANDARD 20)

add_executable(orinwatch src/main.cpp)

install(TARGETS orinwatch DESTINATION /usr/local/bin)

install(FILES orinwatch.service DESTINATION /etc/systemd/system)
