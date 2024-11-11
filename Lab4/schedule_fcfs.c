/**
 * FCFS scheduling
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

// Define head here instead of just declaring it extern
struct node *head = NULL;

// Add the missing add function
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    
    insert(&head, newTask);
}

void schedule() {
    struct node *current = head; 

    while(current != NULL){
        Task* task = current -> task;

        run(task, task->burst);

        current = current->next; 
    }
}