#ifndef __ACTIVITES_H__
#define __ACTIVITES_H__


void activities(struct Process** head);

void RemoveProc(struct Process** head,int pid);

struct Process* merge(struct Process* left, struct Process* right);

void mergeSort(struct Process** head);

void split(struct Process* source, struct Process** front, struct Process** back);
#endif