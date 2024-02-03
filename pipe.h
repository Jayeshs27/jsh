#ifndef __PIPE_H__
#define __PIPE_H__


int Separate_from_pipe(char* sstr,char** new_str, int ncnt);

char** pipeSeparate(char** str, int* cnt_ptr);

bool Redirect(int new, int old);

bool pipeError(char* str, int word_cnt, bool* ptr, int* counter);

bool Pipe_And_Exe(char** single_cmd,char* commands,int ct, bool* pastevents_flag,bool* fg_flag,bool bg_flag);

#endif