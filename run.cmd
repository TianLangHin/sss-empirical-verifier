@echo off
IF "%~3" NEQ "" (
  g++ -D WIDTH=%~1 -D DEPTH=%~2 -std=c++23 theory/heuristic.cpp main.cpp tree/*.hpp -Wall -Wextra -o main.exe
  main.exe %~3
  del main.exe
) ELSE IF "%~2" NEQ "" (
  g++ -D WIDTH=%~1 -D DEPTH=%~2 -std=c++23 theory/heuristic.cpp main.cpp tree/*.hpp -Wall -Wextra -o main.exe
  main.exe
  del main.exe
) ELSE (
  echo Please input width and depth first.
)
