/**
 * FCFS scheduling
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

//Your code and design here
extern struct node *head;

void fcfs_schedule() {
    struct node *current = head; 

    while(current != NULL){
        Task* task = current -> task;

        run(task->name, task->burst);

        current = current->next; 
    }
}