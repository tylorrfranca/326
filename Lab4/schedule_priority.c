/*
 * Priority scheduling
 */
#include <stdlib.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *head = NULL;

void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    
    insert(&head, newTask);
}

void sort_by_priority() {
    if (head == NULL || head->next == NULL) return;

    struct node *sorted = NULL;
    struct node *current = head;

    while (current != NULL) {
        struct node *next = current->next;
        
        if (sorted == NULL || current->task->priority > sorted->task->priority) {
            current->next = sorted;
            sorted = current;
        } else {
            struct node *temp = sorted;
            while (temp->next != NULL && 
                   temp->next->task->priority >= current->task->priority) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

void schedule() {
    sort_by_priority();
    struct node *current = head;

    while(current != NULL) {
        Task *task = current->task;
        run(task, task->burst);
        current = current->next;
    }
}