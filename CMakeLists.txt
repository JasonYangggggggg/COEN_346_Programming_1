cmake_minimum_required(VERSION 3.26)
project(A1 C)

set(CMAKE_C_STANDARD 11)
set( CMAKE_BUILD_TYPE Debug )

add_executable(A1 src/main.c
        include/Pid_Manager.h
        src/Pid_Manager.c
        include/Round_Robin.h
        src/Round_Robin.c
        include/Pcb.h
        src/Pcb.c
        include/Queue.h
        src/Queue.c
        src/Fcfs.c
        include/Fcfs.h
        src/Prio_RR.c
        include/Prio_RR.h)
