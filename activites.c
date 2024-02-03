#include "headers.h"


struct Process* merge(struct Process* left, struct Process* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;

    struct Process* result = NULL;

    if (left->pid <= right->pid)
    {
        result = left;
        result->next = merge(left->next, right);
    } 
    else 
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

void split(struct Process* source, struct Process** front, struct Process** back) {
    struct Process* fast;
    struct Process* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void mergeSort(struct Process** head) {
    struct Process* headRef = *head;
    struct Process* a;
    struct Process* b;

    if ((headRef == NULL) || (headRef->next == NULL))
    {
        return;
    }

    split(headRef, &a, &b);
    mergeSort(&a);
    mergeSort(&b);

    *head = merge(a, b);
}

void activities(struct Process** head)
{
    if (*head == NULL)
    {
        return;
    }

    struct Process* current = *head;
    struct Process* prev = NULL;
    mergeSort(&bg_list);
    while (current != NULL) 
    {
        printf("%d : %s - %s\n", current->pid, current->name, current->state);
    
        current = current->next;
    }
}