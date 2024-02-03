#ifndef __SEEK_H__
#define __SEEK_H__


void Seek_recursive(char* Path,char** Exec,char* target, int flag1, int flag2);

char* RemoveExten(char* str);

void cat(char* filePath);

void Execute_seek(char* Path,char* target, int flag1, int flag2);

bool Doub_strcmp(char* a1, char* a2, char* b1, char* b2);

bool Not_D_flags(char* str);

void seek(char** cmd_seq, int cmd_cnt);

#endif