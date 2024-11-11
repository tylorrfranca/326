/**
 * Round-robin scheduling
 */

#include <stdlib.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

// Define head instead of declaring it extern
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
    
    while (current != NULL) {
        Task *task = current->task;
        int time_slice = (task->burst > QUANTUM) ? QUANTUM : task->burst;
        
        run(task, time_slice);
        task->burst -= time_slice;
        
        if (task->burst > 0) {
            // Move current task to end of list
            struct node *temp = current;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = current;
            head = current->next;
            current->next = NULL;
            current = head;
        } else {
            // Remove completed task
            head = current->next;
            free(current->task);
            free(current);
            current = head;
        }
    }
}