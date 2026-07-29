@echo off

g++ -std=c++23 main.cpp tree/node.hpp theory/*.hpp theory/*.cpp algorithm/*.hpp -o main.exe
main.exe
del main.exe
