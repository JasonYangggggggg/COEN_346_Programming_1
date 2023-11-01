//
// Created by Eve Gagnon on 2023-10-15.
//

#ifndef A1_ROUND_ROBIN_H
#define A1_ROUND_ROBIN_H

#include "Pcb.h"
#include "Queue.h"



int executeRR(struct Pcb *p_list, int quantum, int exec_time, int num_pid);

#endif //A1_ROUND_ROBIN_H
