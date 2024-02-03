#ifndef __PROMPT_H
#define __PROMPT_H


void PrintError(char* str);

void prompt();

void Seq_Run(char** cmd_seq, int cmd_cnt);

bool Separate(char* commands);

extern int currfg;
extern char firstfg[128];
struct BgProcess {
    pid_t pid;
    int exit_status;
};



#endif