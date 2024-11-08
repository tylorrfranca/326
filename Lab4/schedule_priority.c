/*
 * Priority scheduling
 */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

//Your code and design here
extern struct node *head;

void sort_by_priority(struct node **head) {
    struct node *sorted = NULL; 
    struct node *current = *head;

    while(current != NULL) {
        stuct node *next = current->next; 
        

        if(sorted == NULL || current->task->priority < sorted->task->priority){
            current -> next = sorted; 
            sorted = current; 
        } else {
            struct node *temp = sorted; 
            while(temp -> next != NULL && temp->next->priority <= current->task->priority){
                temp = temp->next; 
            }
            current->next = temp->next; 
            temp->next = current; 
        }

        current = next; 
    }
    *head = sorted; 
}

void fcfs_schedule() {

    sort_by_priority(&head);

    struct node *current = head; 

    while(current != NULL){
        Task* task = current -> task;

        run(task->name, task->burst);

        current = current->next; 
    }
}