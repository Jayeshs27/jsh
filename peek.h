#ifndef PEEK_H_
#define PEEK_H_


static int myCompare(const void* a, const void* b);

void sort(char** arr, int n);

char** MakeStringArr(int n,int size);

void printFileInfo(char *file, struct stat *fileStat);

bool isFlag(char* str);

bool IsTHIR_flag(char* str);

bool isDir(char* PATH);

bool isDirect(char* PATH);

void Execute_cmd(char* Path,int Allflag, int infoFlag);

void peek(char** cmd_seq, int cmd_cnt);



#endif