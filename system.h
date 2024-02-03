#ifndef __SYSTEM_H__
#define __SYSTEM_H__

struct Process {
    int pid;
    int status;
    char* name;
    char* state; 
    struct Process* next;
};

struct ProcInfo {
    int pid;
    char* name;
    struct ProcInfo* next;
};



extern struct Process* bg_list;

void handleChildExit(int signum);

void ChangeState(struct Process** head, int pid, int status, char* state);

void addProcess(struct Process** head, int pid, char* name, char* State);

// void Check_bg_Processes(struct Process** head);

void Excute_sys(char** one_cmd, int ct, int bg_flag);

void Check_bg_Processes(struct Process** head);

#endif