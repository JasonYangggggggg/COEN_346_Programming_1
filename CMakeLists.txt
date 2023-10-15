cmake_minimum_required(VERSION 3.26)
project(A1 C)

set(CMAKE_C_STANDARD 11)

add_executable(A1 main.c
        pid_manager.h
        pid_manager.c
        scheduler.h
        scheduler.c
        pcb.h
        pcb.c)
